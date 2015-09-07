/**
 * Vector3D.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * Basic 3 dimensional vector x,y,z
 */

#ifndef RAYTRACER_VECTOR3D_H
#define RAYTRACER_VECTOR3D_H

#include <iostream>
#include <math.h>

class Vector3D {
public:
    //Default constructor
    Vector3D(double x = 0.0, double y = 0.0, double z = 0.0);

    //Vector values
    double x;
    double y;
    double z;

    //Math operator overrides
    Vector3D operator+(const Vector3D &other);
    Vector3D operator-(const Vector3D &other);
    Vector3D operator*(const double &other);
    Vector3D operator*(const int &other);

    bool operator==(const Vector3D &other);
    bool operator<(const Vector3D &other);

    double magnitude() const
    {
        return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    }

    Vector3D unit()
    {
        double mag = magnitude();
        return Vector3D(x/mag, y/mag, z/mag);
    }

    //Printable interface
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& vec)
    {
        os << vec.x << ',' << vec.y << ',' << vec.z;
        return os;
    }
};

#endif //RAYTRACER_VECTOR3D_H
