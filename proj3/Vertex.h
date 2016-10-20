/**
 * Vertex.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * A vertex point
 */

#ifndef ROOTPROJ_VERTEX_H_H
#define ROOTPROJ_VERTEX_H_H

#include <Eigen/Dense>

/**
 * vertex
 * point in space
 */
class vertex {
public:
    vertex():camera_pos(NULL){}
    virtual ~vertex(){}

    //3D point in space
    Eigen::Vector3d pos;

    //3D point in space
    Eigen::Vector4d *camera_pos;

    //Tangent and bitangent of this point
    double tangent, bitangent;

    vertex(const Eigen::Vector3d &vec){
        pos = vec;
    }
};

#endif //ROOTPROJ_VERTEX_H_H