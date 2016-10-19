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
#include "Vector3D.h"
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
    void addMaterial(Material &mat);

    /**
     * finalize
     * precalculate values for hit detection
     */
    void finalize();

    /**
     * normal
     * returns normal vector relative to given vector
     */
    virtual Vector3D normal(const Vector3D &vec) const;

    virtual void breakIntoTriangles() {
        if(vertices.size() == 3) return;
        if(vertices.size() != 4) return;

        vertex one = vertices[0];
        vertex two = vertices[1];
        vertex three = vertices[2];
        vertex four = vertices[3];

        vertices.clear();
    }
private:
    //Normal vector
    Vector3D norm;
    //Tangent vector
    Vector3D tangent;
    //Bitangent vector
    Vector3D bitangent;

    double vecNormal;
};


#endif //RAYTRACER_POLYGON_H
