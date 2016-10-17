//
// Created by Christopher on 10/14/2016.
//

#ifndef ROOTPROJ_PIPELINE_H
#define ROOTPROJ_PIPELINE_H

#include <Eigen/Dense>
#include <utility>
#include <algorithm>
#include <stdio.h>
#include "World.h"
#include "ViewDetails.h"
#include "Actor.h"
#include "Polygon.h"
#include "Color.h"

class Pipeline {
private:
    World* m_world;
    Eigen::Matrix4d M_vp;
    Eigen::Matrix4d M_ortho;
    Eigen::Matrix4d M_persp;
    Eigen::Matrix4d M_camera;
    Eigen::Matrix4d M_matrix;

protected:
    void GenerateViewMatrix(){
        ViewDetails *view = m_world->getRenderer();

        M_vp(0,0) = ((double)view->width())/2.0;
        M_vp(1,1) = ((double)view->height())/2.0;
        M_vp(0,3) = ((double)view->width() - 1.0)/2.0;
        M_vp(1,3) = ((double)view->height() - 1.0)/2.0;
        M_vp(2,2) = 1.0;
        M_vp(3,3) = 1.0;

        std::cout << "ViewMatrix" << std::endl
                  << M_vp << std::endl;
    }
    void GenerateOrthogonalMatrix(){
        ViewDetails *view = m_world->getRenderer();

        //X
        M_ortho(0,0) = 2.0/(view->right()-view->left());
        M_ortho(0,3) = -(view->right()+view->left())/(view->right()-view->left());

        //Y
        M_ortho(1,1) = 2.0/(view->top()-view->bottom());
        M_ortho(1,3) = -(view->top()+view->bottom())/(view->top()-view->bottom());

        //Z
        M_ortho(2,2) = 2.0/(view->nearPlane()-view->farPlane());
        M_ortho(2,3) = -(view->nearPlane()+view->farPlane())/(view->nearPlane()-view->farPlane());

        std::cout << "Orthogonal" << std::endl
                  << M_ortho << std::endl;
    }

    void GenerateCameraMatrix(){
        ViewDetails *view = m_world->getRenderer();

        Vector3D oldLook = view->eye() - view->spot();

        Eigen::Vector3d look(oldLook.x, oldLook.y, oldLook.z);
        //LOOK
        Eigen::Vector3d w = -look.normalized();


        Eigen::Vector3d upPoint(view->up().x, view->up().y, view->up().z);
        Eigen::Vector3d t = look.cross(upPoint);
        //RIGHT
        Eigen::Vector3d u = t.cross(w).normalized();

        //UP
        Eigen::Vector3d v = w.cross(u);

        M_camera(0,0) = view->u().x;
        M_camera(0,1) = view->u().y;
        M_camera(0,2) = view->u().z;

        M_camera(1,0) = view->v().x;
        M_camera(1,1) = view->v().y;
        M_camera(1,2) = view->v().z;

        M_camera(2,0) = view->w().x;
        M_camera(2,1) = view->w().y;
        M_camera(2,2) = view->w().z;

        M_camera(3,3) = 1.0;

        std::cout << "Pre-Eye Camera" << std::endl
                  << M_camera << std::endl;

        Eigen::Matrix4d eye = Eigen::Matrix4d::Identity();

        eye(0,3) = -view->eye().x;
        eye(1,3) = -view->eye().y;
        eye(2,3) = -view->eye().z;

        std::cout << "Eye" << std::endl
                  << eye << std::endl;

        M_camera = M_camera * eye;

        std::cout << "Camera" << std::endl
                  << M_camera << std::endl;
    }

    void GeneratePerspectiveMatrix(){
        ViewDetails *view = m_world->getRenderer();

        M_persp(0,0) = view->nearPlane();

        M_persp(1,1) = view->nearPlane();

        M_persp(2,2) = view->nearPlane()+view->farPlane();

        M_persp(2,3) = -view->farPlane()*view->nearPlane();

        M_persp(3,2) = 1.0;


        std::cout << "P-Matrix" << std::endl
                  << M_persp << std::endl;

        M_persp = M_ortho * M_persp;

        std::cout << "Perspective"
                  << std::endl << M_persp << std::endl;
    }
public:

    Pipeline(World *world): m_world(world){
        ViewDetails *view = m_world->getRenderer();

        M_vp = Eigen::Matrix4d::Zero();
        M_ortho = Eigen::Matrix4d::Zero();
        M_persp = Eigen::Matrix4d::Zero();
        M_camera = Eigen::Matrix4d::Zero();
        M_matrix = Eigen::Matrix4d::Zero();

        GenerateViewMatrix();
        GenerateOrthogonalMatrix();
        GeneratePerspectiveMatrix();
        GenerateCameraMatrix();

        M_matrix = M_vp * M_persp * M_camera;

        std::cout << "M-Matrix" << std::endl
                  << M_matrix << std::endl;

    }

    void VertexProcessing(){
        const std::vector<Actor*>* actors = m_world->Actors();

        for(std::vector<Actor*>::const_iterator it = actors->begin(); it < actors->end(); it++){
            Actor *actor = (*it);

            for(std::vector<vertex>::iterator jt = actor->getVerticies()->begin(); jt < actor->getVerticies()->end(); jt++){
                Eigen::Vector4d vec;
                vec(0) = jt->pos.x;
                vec(1) = jt->pos.y;
                vec(2) = jt->pos.z;
                vec(3) = 1.0;
                vec = M_matrix * vec;

                //Have to do this to work with windows
                jt->camera_pos = new Eigen::Vector4d(vec);
                //std::cout << *(jt->camera_pos) << std::endl << std::endl;
            }
        }
    }

    void Clipping(){
        return;
    }

    void Rasterization(){
        const std::vector<Actor*>* actors = m_world->Actors();

        for(std::vector<Actor*>::const_iterator it = actors->begin(); it < actors->end(); it++) {
            Actor *actor = (*it);

            for (std::vector<vertex>::iterator jt = actor->getVerticies()->begin(); jt < actor->getVerticies()->end(); jt++) {
                Eigen::Vector4d *vec = jt->camera_pos;

                //std::cout << vec->coeffRef(0) << "/" << M_matrix(0,3) << std::endl;
            }
        }


    }

    void FragmentProcessing(){
        return;
    }

    void Blending(std::string outputFile){
        ViewDetails *view = m_world->getRenderer();

        //Data array
        unsigned char pixels[view->height()][view->width()][3];

        Color bgColor = Color(view->background());
        for(int y = 0; y < view->height(); y++){
            for(int x = 0; x < view->width(); x++){
                // assign color
                pixels[y][x][0] = bgColor.R;
                pixels[y][x][1] = bgColor.G;
                pixels[y][x][2] = bgColor.B;
            }
        }

        const std::vector<Actor*>* actors = m_world->Actors();

        for(std::vector<Actor*>::const_iterator it = actors->begin(); it < actors->end(); it++) {
            Actor *actor = (*it);

            //Figure color of the ray
            Color color = Color(actor->Texture().color);

            std::vector<vertex> verts = *(actor->getVerticies());

            double xValues[] = {
                    verts[0].camera_pos->coeffRef(0)/M_matrix(0,3)*view->width(),
                    verts[1].camera_pos->coeffRef(0)/M_matrix(0,3)*view->width(),
                    verts[2].camera_pos->coeffRef(0)/M_matrix(0,3)*view->width()
            };

            double yValues[] = {
                    verts[0].camera_pos->coeffRef(1)/M_matrix(1,3)*view->height(),
                    verts[1].camera_pos->coeffRef(1)/M_matrix(1,3)*view->height(),
                    verts[2].camera_pos->coeffRef(1)/M_matrix(1,3)*view->height()
            };

            int minX = (int)std::max((double)0.0, std::min(xValues[0],
                                                   std::min(xValues[1],
                                                            xValues[2])));
            int maxX = (int)std::min((double)view->width(), std::max(xValues[0],
                                                             std::max(xValues[1],
                                                                      xValues[2])));

            int minY = (int)std::max((double)0.0, std::min(yValues[0],
                                                   std::min(yValues[1],
                                                            yValues[2])));
            int maxY = (int)std::min((double)view->height(), std::max(yValues[0],
                                                             std::max(yValues[1],
                                                                      yValues[2])));


            for(int y = minY; y < maxY; y++){
                for(int x = minX; x < maxX; x++){
                    double alpha = (yValues[1] - yValues[2])*x + (xValues[2] - xValues[1])*y + xValues[1]*yValues[2] - xValues[2]*yValues[1];
                    double beta = (yValues[2] - yValues[0])*x + (xValues[0] - xValues[2])*y + xValues[2]*yValues[0] - xValues[0]*yValues[2];
                    double gamma = (yValues[0] - yValues[1])*x + (xValues[1] - xValues[0])*y + xValues[0]*yValues[1] - xValues[1]*yValues[0];

                    if(alpha > 0 && beta > 0 && gamma > 0) {
                        // assign color
                        pixels[y][x][0] = color.R;
                        pixels[y][x][1] = color.G;
                        pixels[y][x][2] = color.B;
                    }
                }
            }
        }

        //Given by prompt
        //Open file
        FILE *f = fopen(outputFile.c_str(),"wb");
        //Print information for ppm file
        fprintf(f, "P6\n%d %d\n%d\n", view->width(), view->height(), 255);
        //Write the pixel data to the file
        fwrite(pixels, 1, view->height()*view->width()*3, f);
        //Close the file
        fclose(f);
    }
};


#endif //ROOTPROJ_PIPELINE_H
