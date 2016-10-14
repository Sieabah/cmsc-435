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
     * vertex
     * point in space
     */
    struct vertex {
        //3D point in space
        Vector3D pos;
        //Tangent and bitangent of this point
        double tangent, bitangent;

        /**
         * vertex
         * constructor
         */
        vertex(const Vector3D &vec){pos = vec;}
    };

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
     * addMaterial
     * Add material to polygon
     */
    void addMaterial(Material &mat);

    /**
     * intersect
     * Find if ray intersects with polygon
     */
    virtual const Hit intersect(const Ray &ray) const;

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

private:
    //Normal vector
    Vector3D norm;
    //Tangent vector
    Vector3D tangent;
    //Bitangent vector
    Vector3D bitangent;

    double vecNormal;

    //Vertices of the polygon
    std::vector<vertex> vertices;
};


#endif //RAYTRACER_POLYGON_H
