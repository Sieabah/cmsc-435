/**
 * Polygon.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
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

    /**
     * Destructor
     */
    virtual ~Polygon(){}

    /**
     * addMaterial
     * Add material to polygon
     */
    void addMaterial(Material &mat){
        material = mat;
    };

    /**
     * Precalculate values to detect if ray intersects poly
     */
    void finalize();
private:
    /**
     * Normal Vector
     */
    Eigen::Vector3d norm;

    /**
     * Tangent vector
     */
    Eigen::Vector3d tangent;

    /**
     * Bitangent vector
     */
    Eigen::Vector3d bitangent;

    /**
     * Normal dot product
     */
    double vecNormal;
};


#endif //RAYTRACER_POLYGON_H
