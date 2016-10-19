/**
 * Polygon.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * n-sided polygon
 */

#ifndef RAYTRACER_POLYGON_H
#define RAYTRACER_POLYGON_H

#include <vector>
#include <Eigen/Dense>
#include "Actor.h"
#include "Ray.h"

class Polygon: public Actor {
public:
    /**
     * Default constructor
     * Inlined for simplicity
     */
    Polygon(){};

    ~Polygon(){}

    /**
     * addMaterial
     * Add material to polygon
     */
    void addMaterial(Material &mat){
        material = mat;
    };
private:
    //Normal vector
    Eigen::Vector3d norm;
    //Tangent vector
    Eigen::Vector3d tangent;
    //Bitangent vector
    Eigen::Vector3d bitangent;

    double vecNormal;
};


#endif //RAYTRACER_POLYGON_H
