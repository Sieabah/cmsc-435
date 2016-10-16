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

        M_persp(0,0) = view->nearPlane();
        M_persp(1,1) = view->nearPlane();

        M_persp(2,2) = view->nearPlane() + view->dist();
        M_persp(2,3) = -(view->nearPlane() * view->dist());
        M_persp(3,2) = 1.0;
    }
public:

    Pipeline(World *world): m_world(world){
        GenerateViewMatrix();
        GenerateOrthogonalMatrix();
        GeneratePerspectiveMatrix();
        GenerateCameraMatrix();

        M_matrix = M_vp * M_ortho * M_persp * M_camera;

        std::cout << M_matrix;
    }

    void VertexProcessing(){
        const std::vector<Actor*>* actors = m_world->Actors();

        for(std::vector<Actor*>::const_iterator it = actors->begin(); it < actors->end(); it++){
            Actor *actor = (*it);
            std::for_each(actor->getVerticies()->begin(), actor->getVerticies()->end(),
            [&](const vertex &vert){
                Eigen::Vector4d vec;
                vec(0) = vert.pos.x;
                vec(1) = vert.pos.y;
                vec(2) = vert.pos.z;
                vec(3) = 1.0;
                vert.camera_pos = M_matrix * vec;
            });
        }
    }

    void Clipping(){
        return;
    }

    void Rasterization(){

    }

    void FragmentProcessing(){
        return;
    }

    void Blending(){

    }
};


#endif //ROOTPROJ_PIPELINE_H
