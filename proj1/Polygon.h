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
    Polygon(){}

    /**
     * addVert
     * Add vertex to polygon
     */
    void addVert(Vector3D coord)
    {
        vertices.push_back(coord);
    }
private:
    std::vector<Vector3D> vertices;
};


#endif //RAYTRACER_POLYGON_H
