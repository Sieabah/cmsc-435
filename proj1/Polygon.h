/**
 * Polygon.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * Basic n-sided polygon
 */

#ifndef RAYTRACER_POLYGON_H
#define RAYTRACER_POLYGON_H

#include <vector>
#include "Vector3D.h"

class Polygon {
public:
    /**
     * Default constructor
     * Inlined for simplicity
     */
    Polygon(){};

    /**
     * addVert
     * Add vertex to polygon
     */
    void addVert(Vector3D coord);

    /**
     * intersects
     * Determine if ray point and direction intersect with the polygon
     */
    virtual bool intersects(Vector3D ray, Vector3D direction);

private:
    //Vertices of the polygon
    std::vector<Vector3D> vertices;
};


#endif //RAYTRACER_POLYGON_H
