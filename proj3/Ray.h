/**
 * Ray.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * 3D ray class
 */

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "Vector3D.h"

class Ray {
public:
    /**
     * Ray
     * constructor for creating simple ray
     */
    Ray(Vector3D pos, Vector3D dir, double nearP=1e-4, double farP=INFINITY):
            position(pos), direction(dir), nearPlane(nearP), farPlane(farP) {};

    /**
     * Getters
     */
    Vector3D pos() const{return position;}
    Vector3D dir() const{return direction;}
    double near() const{return nearPlane;}
    double far() const{return farPlane;}

    /**
     * Setters
     */
    double far(double len){farPlane = len;}
private:
    //Ray origin
    Vector3D position;

    //Ray direction
    Vector3D direction;

    //Near/far clipping planes
    double nearPlane;
    double farPlane;
};


#endif //RAYTRACER_RAY_H

