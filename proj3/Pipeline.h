//
// Created by Christopher on 10/14/2016.
//

#ifndef ROOTPROJ_PIPELINE_H
#define ROOTPROJ_PIPELINE_H

#include <Eigen/Dense>
#include <utility>
#include <algorithm>
#include "World.h"
#include "ViewDetails.h"
#include "Actor.h"
#include "Polygon.h"
#include "Color.h"

class Pipeline {
private:
    World* m_world;
    Eigen::Matrix4d M_vp = Eigen::Matrix4d::Zero();
    Eigen::Matrix4d M_ortho = Eigen::Matrix4d::Zero();
    Eigen::Matrix4d M_persp = Eigen::Matrix4d::Zero();
    Eigen::Matrix4d M_camera = Eigen::Matrix4d::Zero();
    Eigen::Matrix4d M_matrix = Eigen::Matrix4d::Zero();

protected:
    void GenerateViewMatrix(){
        std::pair<unsigned int, unsigned int> resolution = m_world->getRenderer()->resolution();

        M_vp(0,0) = ((double)resolution.first)/2.0;
        M_vp(1,1) = ((double)resolution.second)/2.0;
        M_vp(0,3) = ((double)resolution.first - 1.0)/2.0;
        M_vp(1,3) = ((double)resolution.second - 1.0)/2.0;
        M_vp(2,2) = 1.0;
        M_vp(3,3) = 1.0;
    }
    void GenerateOrthogonalMatrix(){
        ViewDetails *view = m_world->getRenderer();
        double nearPlane = view->nearPlane();
        double farPlane = view->farPlane();
        M_ortho(0,0) = 2.0/(view->right()-view->left());
        M_ortho(0,3) = -(view->right()+view->left())/(view->right()-view->left());

        M_ortho(1,1) = 2.0/(view->top()-view->bottom());
        M_ortho(1,3) = -(view->top()+view->bottom())/(view->top()-view->bottom());

        M_ortho(2,2) = 2.0/(nearPlane-farPlane);
        M_ortho(2,3) = -(nearPlane+farPlane)/(nearPlane-farPlane);

    }

    void GenerateCameraMatrix(){
        ViewDetails *view = m_world->getRenderer();
        Eigen::Vector3d g(view->d().x, view->d().y, view->d().z);
        Eigen::Vector3d w = -g/g.norm();

        Eigen::Vector3d t(view->up().x, view->up().y, view->up().z);
        Eigen::Vector3d u = t.cross(w);
        u = u/u.norm();

        Eigen::Vector3d v = w.cross(u);

        M_camera(0,0) = u(0);
        M_camera(0,1) = u(1);
        M_camera(0,2) = u(2);

        M_camera(1,0) = v(0);
        M_camera(1,1) = v(1);
        M_camera(1,2) = v(2);

        M_camera(2,0) = w(0);
        M_camera(2,1) = w(1);
        M_camera(2,2) = w(2);

        M_camera(0,3) = -view->eye().x;
        M_camera(1,3) = -view->eye().y;
        M_camera(2,3) = -view->eye().z;

        M_camera(3,3) = 1.0;
    }

    void GeneratePerspectiveMatrix(){
        ViewDetails *view = m_world->getRenderer();

        M_persp(0,0) = (2*view->nearPlane())/(view->right()-view->left());
        M_persp(0,2) = (view->right()+view->left())/(view->left()-view->right());

        M_persp(1,1) = (2*view->nearPlane())/(view->top()-view->bottom());
        M_persp(1,2) = (view->top()+view->bottom())/(view->bottom()-view->top());


        M_persp(2,2) = (view->nearPlane() + view->farPlane())/(view->nearPlane()-view->farPlane());
        M_persp(2,3) = (2*view->farPlane()*view->nearPlane())/(view->farPlane()-view->nearPlane());

        M_persp(3,2) = 1.0;
    }
public:

    Pipeline(World *world): m_world(world){
        GenerateViewMatrix();
        GenerateOrthogonalMatrix();
        GeneratePerspectiveMatrix();
        GenerateCameraMatrix();

        M_matrix = M_vp * M_persp * M_camera;

        std::cout << M_matrix;
    }

    void VertexProcessing(){
        const std::vector<Actor*>* actors = m_world->Actors();

        for(std::vector<Actor*>::const_iterator it = actors->begin(); it < actors->end(); it++){
            Actor *actor = (*it);

            for(auto jt = actor->getVerticies()->begin(); jt < actor->getVerticies()->end(); jt++){
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

                vec->coeffRef(0,0) = vec->coeffRef(0,0)/ M_matrix(0,3);
                vec->coeffRef(1,0) = vec->coeffRef(1,0)/ M_matrix(1,3);
                vec->coeffRef(2,0) = vec->coeffRef(2,0)/ M_matrix(2,3);
                vec->coeffRef(3,0) = vec->coeffRef(3,0)/ M_matrix(3,3);
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
                    verts[0].camera_pos->coeffRef(0)*view->width(),
                    verts[1].camera_pos->coeffRef(0)*view->width(),
                    verts[2].camera_pos->coeffRef(0)*view->width()
            };

            double yValues[] = {
                    verts[0].camera_pos->coeffRef(1)*view->height(),
                    verts[1].camera_pos->coeffRef(1)*view->height(),
                    verts[2].camera_pos->coeffRef(1)*view->height()
            };

            int minX = (int)std::max(0.0, std::min(xValues[0],
                                                   std::min(xValues[1],
                                                            xValues[2])));
            int maxX = (int)std::min((double)view->width(), std::max(xValues[0],
                                                             std::max(xValues[1],
                                                                      xValues[2])));

            int minY = (int)std::max(0.0, std::min(yValues[0],
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
