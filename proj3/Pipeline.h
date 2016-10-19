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

typedef std::vector<std::vector<std::pair<Eigen::Vector3d, double>>> raster;
typedef std::vector<std::pair<Eigen::Vector3d, double>> pixelPairs;

class Pipeline {
private:
    World* m_world;
    Eigen::Matrix4d M_vp;
    Eigen::Matrix4d M_ortho;
    Eigen::Matrix4d M_persp;
    Eigen::Matrix4d M_camera;
    Eigen::Matrix4d M_matrix;

    const bool DEBUG_OUTPUT;
    const bool OUTPUT_Z_BUFFER;

protected:
    void GenerateViewMatrix(){
        ViewDetails *view = m_world->getRenderer();

        M_vp(0,0) = ((double)view->width())/2.0;
        M_vp(1,1) = ((double)view->height())/2.0;
        M_vp(0,3) = ((double)view->width() - 1.0)/2.0;
        M_vp(1,3) = ((double)view->height() - 1.0)/2.0;
        M_vp(2,2) = 1.0;
        M_vp(3,3) = 1.0;

        if(DEBUG_OUTPUT)
            std::cout << "ViewMatrix" << std::endl << M_vp << std::endl;
    }
    void GenerateOrthogonalMatrix() {
        ViewDetails *view = m_world->getRenderer();

        //X
        M_ortho(0, 0) = 2.0 / (view->right() - view->left());
        M_ortho(0, 3) = -(view->right() + view->left()) / (view->right() - view->left());

        //Y
        M_ortho(1, 1) = 2.0 / (view->top() - view->bottom());
        M_ortho(1, 3) = -(view->top() + view->bottom()) / (view->top() - view->bottom());

        //Z
        M_ortho(2, 2) = 2/(view->nearPlane() - view->farPlane());
        M_ortho(2, 3) = -(view->nearPlane() + view->farPlane()) / (view->nearPlane() - view->farPlane());

        M_ortho(3, 3) = 1.0;

        if(DEBUG_OUTPUT)
            std::cout << "Orthogonal" << std::endl << M_ortho << std::endl;
    }

    void GenerateCameraMatrix(){
        ViewDetails *view = m_world->getRenderer();

        //LOOK
        Eigen::Vector3d w = view->w();

        //RIGHT
        Eigen::Vector3d u = view->u();

        //UP
        Eigen::Vector3d v = view->v();

        M_camera(0,0) = u(0);
        M_camera(0,1) = u(1);
        M_camera(0,2) = u(2);

        M_camera(1,0) = v(0);
        M_camera(1,1) = v(1);
        M_camera(1,2) = v(2);

        M_camera(2,0) = w(0);
        M_camera(2,1) = w(1);
        M_camera(2,2) = w(2);

        M_camera(3,3) = 1.0;

        if(DEBUG_OUTPUT)
            std::cout << "Pre-Eye Camera" << std::endl << M_camera << std::endl;

        Eigen::Matrix4d eyeMatrix = Eigen::Matrix4d::Identity();

        eyeMatrix(0,3) = -view->eye()(0);
        eyeMatrix(1,3) = -view->eye()(1);
        eyeMatrix(2,3) = -view->eye()(2);

        if(DEBUG_OUTPUT)
            std::cout << "Eye" << std::endl << eyeMatrix << std::endl;

        M_camera = M_camera * eyeMatrix;

        if(DEBUG_OUTPUT)
            std::cout << "Camera" << std::endl << M_camera << std::endl;
    }

    void GeneratePerspectiveMatrix(){
        ViewDetails *view = m_world->getRenderer();

        M_persp(0,0) = view->nearPlane();

        M_persp(1,1) = view->nearPlane();

        M_persp(2,2) = view->nearPlane()+view->farPlane();

        M_persp(2,3) = -view->farPlane()*view->nearPlane();

        M_persp(3,2) = 1.0;


        if(DEBUG_OUTPUT)
            std::cout << "P-Matrix" << std::endl << M_persp << std::endl;

        M_persp = M_ortho * M_persp;

        if(DEBUG_OUTPUT)
            std::cout << "Perspective" << std::endl << M_persp << std::endl;
    }

    void GenerateMMatrix(){
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

        if(DEBUG_OUTPUT)
            std::cout << "M-Matrix" << std::endl << M_matrix << std::endl;
    }

    //Data array
    raster pixels;
public:

    Pipeline(World *world, bool zBuffer=false, bool debug=false): m_world(world),
    DEBUG_OUTPUT(debug), OUTPUT_Z_BUFFER(zBuffer){
        if(DEBUG_OUTPUT)
            std::cout << "Pipeline" << std::endl;

        ViewDetails *view = m_world->getRenderer();

        pixels.resize(view->height());
        for(raster::iterator i = pixels.begin(); i < pixels.end(); i++){
            i->resize(view->width());
            for(pixelPairs::iterator j = i->begin(); j < i->end(); j++){
                j->first = Eigen::Vector3d(view->background().x, view->background().y, view->background().z);
                j->second = view->farPlane();
            }
        }
    }

    void run(std::string outputFile){
        GenerateMMatrix();

        std::cout << "Step 1/5 VP" << std::endl;
        VertexProcessing();
        std::cout << "Step 2/5 C" << std::endl;
        Clipping();
        std::cout << "Step 3/5 R" << std::endl;
        Rasterization();
        std::cout << "Step 4/5 FP" << std::endl;
        FragmentProcessing();
        std::cout << "Step 5/5 B" << std::endl;
        Blending();
        Save(outputFile);
    }
protected:

    void VertexProcessing(){
        if(DEBUG_OUTPUT)
            std::cout << "VertexProcessing" << std::endl;

        const std::vector<Actor*>* actors = m_world->Actors();

        for(std::vector<Actor*>::const_iterator it = actors->begin(); it < actors->end(); it++){
            Actor *actor = (*it);

            for(std::vector<vertex>::iterator jt = actor->getVerticies()->begin(); jt < actor->getVerticies()->end(); jt++){
                Eigen::Vector4d vec(jt->pos.x, jt->pos.y, jt->pos.z, 1.0);
                vec = M_matrix * vec;

                //Have to do this to work with windows
                jt->camera_pos = new Eigen::Vector4d(vec);
            }
        }
    }

    void Clipping(){
        return;
    }

private:
    double raster_f12(double x, double y, Eigen::Vector3d &xValues, Eigen::Vector3d &yValues){
        return (yValues[1] - yValues[2])*x + (xValues[2] - xValues[1])*y + xValues[1]*yValues[2] - xValues[2]*yValues[1];
    }
    double raster_f20(double x, double y, Eigen::Vector3d &xValues, Eigen::Vector3d &yValues){
        return (yValues[2] - yValues[0])*x + (xValues[0] - xValues[2])*y + xValues[2]*yValues[0] - xValues[0]*yValues[2];
    }

    double raster_f01(double x, double y, Eigen::Vector3d &xValues, Eigen::Vector3d &yValues) {
        return (yValues[0] - yValues[1])*x + (xValues[1] - xValues[0])*y + xValues[0]*yValues[1] - xValues[1]*yValues[0];
    }

protected:
    void Rasterization(){
        if(DEBUG_OUTPUT)
            std::cout << "Rasterization" << std::endl;

        ViewDetails *view = m_world->getRenderer();

        const std::vector<Actor*>* actors = m_world->Actors();

        view->getLight(0);

        double minmax[] = {0, 0, 0, 0};

        for(std::vector<Actor*>::const_iterator it = actors->begin(); it < actors->end(); it++) {
            Actor *actor = (*it);

            std::vector<vertex> verts = *(actor->getVerticies());

            Eigen::Vector3d xValues(
                    verts[0].camera_pos->coeffRef(0)/verts[0].camera_pos->coeffRef(3),
                    verts[1].camera_pos->coeffRef(0)/verts[1].camera_pos->coeffRef(3),
                    verts[2].camera_pos->coeffRef(0)/verts[2].camera_pos->coeffRef(3)
            );

            Eigen::Vector3d yValues(
                    verts[0].camera_pos->coeffRef(1)/verts[0].camera_pos->coeffRef(3),
                    verts[1].camera_pos->coeffRef(1)/verts[1].camera_pos->coeffRef(3),
                    verts[2].camera_pos->coeffRef(1)/verts[2].camera_pos->coeffRef(3)
            );

            Eigen::Vector3d zValues(
                    verts[0].camera_pos->coeffRef(2)/verts[0].camera_pos->coeffRef(3),
                    verts[1].camera_pos->coeffRef(2)/verts[1].camera_pos->coeffRef(3),
                    verts[2].camera_pos->coeffRef(2)/verts[2].camera_pos->coeffRef(3)
            );

            //Scale
            xValues = xValues;
            yValues = yValues;
            zValues = zValues;

            int minX = (int)floor(std::max((double)0.0, xValues.minCoeff()));
            int maxX = (int)ceil(std::min((double)view->width(), xValues.maxCoeff()));

            minmax[0] = xValues.minCoeff() < minmax[0] ? xValues.minCoeff() : minmax[0];
            minmax[2] = xValues.maxCoeff() > minmax[2] ? xValues.maxCoeff() : minmax[2];

            int minY = (int)floor(std::max((double)0.0, yValues.minCoeff()));
            int maxY = (int)ceil(std::min((double)view->height(), yValues.maxCoeff()));

            minmax[1] = yValues.minCoeff() < minmax[1] ? yValues.minCoeff() : minmax[1];
            minmax[3] = yValues.maxCoeff() > minmax[3] ? yValues.maxCoeff() : minmax[3];

            //Triangle coloring
            //@todo per-pixel coloring
            Eigen::Vector3d color(actor->Texture().color.x, actor->Texture().color.y, actor->Texture().color.z);

            for(int y = minY; y < maxY; y++){
                for(int x = minX; x < maxX; x++){
                    double alpha = raster_f12(x,y, xValues, yValues)/raster_f12(xValues(0), yValues(0), xValues, yValues);
                    double beta = raster_f20(x,y, xValues, yValues)/raster_f20(xValues(1), yValues(1), xValues, yValues);
                    double gamma = raster_f01(x,y, xValues, yValues)/raster_f01(xValues(2), yValues(2), xValues, yValues);

                    if(alpha > 0 && beta > 0 && gamma > 0) {
                        double zbuffer = zValues[0] * alpha + zValues[1] * beta + zValues[2] * gamma;

                        if(zbuffer < pixels[y][x].second) {
                            // assign color
                            //Trippy Color Assignment
                            //pixels[y][x].first = Eigen::Vector3d(color(0)*alpha, color(1)*beta, color(2)*gamma);

                            pixels[y][x].first = color;
                            pixels[y][x].second = zbuffer;
                        }
                    }
                }
            }
        }

        if(DEBUG_OUTPUT) {
            std::cout << "min: " << minmax[0] << ", " << minmax[1] << std::endl;
            std::cout << "max: " << minmax[2] << ", " << minmax[3] << std::endl;
        }
    }

    void FragmentProcessing(){
        if(DEBUG_OUTPUT)
            std::cout << "Fragment Processing" << std::endl;
        return;
    }

    void Blending(){
        if(DEBUG_OUTPUT)
            std::cout << "Blending" << std::endl;
    }

private:
    double zBuffer_min = 0;
    double zBuffer_max = 500;
public:
    void defineZBuffer(double min, double max){
        zBuffer_max = max;
        zBuffer_min = min;
    }

protected:

    void Save(std::string outputFile){
        if(DEBUG_OUTPUT)
            std::cout << "Saving to file " << outputFile << std::endl;

        ViewDetails *view = m_world->getRenderer();

        unsigned char output[view->width()][view->height()][3];

        double maxZ, minZ = maxZ = view->nearPlane();

        int yOffset = view->height()-1;
        for(int y = 0; y < view->height(); y++){
            for(int x = 0; x < view->width(); x++){

                if(OUTPUT_Z_BUFFER){
                    output[y][x][0] = Color::Convert((pixels[yOffset-y][x].second-minZ)/maxZ);
                    if(pixels[yOffset-y][x].second == view->farPlane())
                        pixels[yOffset-y][x].second = minZ;
                    output[y][x][1] = Color::Convert((pixels[yOffset-y][x].second-minZ)/maxZ);
                    output[y][x][2] = Color::Convert((pixels[yOffset-y][x].second-minZ)/maxZ);
                } else {
                    Color color(pixels[yOffset-y][x].first);

                    output[y][x][0] = color.R;
                    output[y][x][1] = color.G;
                    output[y][x][2] = color.B;
                }
            }
        }

        //Given by prompt
        //Open file
        FILE *f = fopen(outputFile.c_str(),"wb");
        //Print information for ppm file
        fprintf(f, "P6\n%d %d\n%d\n", view->width(), view->height(), 255);
        //Write the pixel data to the file
        fwrite(output, 1, view->height()*view->width()*3, f);
        //Close the file
        fclose(f);
    }
};


#endif //ROOTPROJ_PIPELINE_H
