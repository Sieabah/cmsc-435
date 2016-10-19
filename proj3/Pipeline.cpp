//
// Created by Christopher on 10/14/2016.
//

#include "Pipeline.h"


void Pipeline::GenerateViewMatrix() {
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

void Pipeline::GenerateOrthogonalMatrix() {
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

void Pipeline::GenerateCameraMatrix() {
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

void Pipeline::GeneratePerspectiveMatrix() {
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

void Pipeline::GenerateMMatrix() {
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
