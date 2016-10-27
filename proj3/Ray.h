/**
 * Ray.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * 3D ray class
 */

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include <Eigen/Dense>

class Ray {
public:
    /**
     * Ray
     * constructor for creating simple ray
     */
    Ray(Eigen::Vector3d pos, Eigen::Vector3d dir, double nearP=1e-4, double farP=INFINITY):
            position(pos), direction(dir), nearPlane(nearP), farPlane(farP) {};

    /**
     * Getters
     */
    Eigen::Vector3d pos() const{return position;}
    Eigen::Vector3d dir() const{return direction;}
    double near() const{return nearPlane;}
    double far() const{return farPlane;}

    /**
     * Setters
     */
    double far(double len){farPlane = len;}
private:
    /**
     * Ray origin
     */
    Eigen::Vector3d position;

    /**
     * Ray direction
     */
    Eigen::Vector3d direction;

    /**
     * Near clipping plane
     */
    double nearPlane;

    /**
     * Far clipping plane
     */
    double farPlane;
};


#endif //RAYTRACER_RAY_H

