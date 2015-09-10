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
#include <exception>

class Vector3D {
public:
    //Default constructor
    Vector3D(double x = 0.0, double y = 0.0, double z = 0.0);
    Vector3D(const Vector3D &other)
    : x(other.x), y(other.y), z(other.z){}

    //Vector values
    double x;
    double y;
    double z;

    //Math operator overrides
    Vector3D operator+(const Vector3D &other);
    Vector3D operator-(const Vector3D &other);
    Vector3D operator*(const double &other);
    Vector3D operator*(const int &other);

    double operator*(const Vector3D &other)
    {
        return Vector3D::dot(*this, other);
    }

    double operator^(int value)
    {
        if(value != 2)
            throw "Cannot raise vector to power given";

        return Vector3D::dot(*this, *this);
    }

    bool operator==(const Vector3D &other);
    bool operator<(const Vector3D &other);

    static double dot(const Vector3D &vec, const Vector3D &oth)
    {
        return vec.x*oth.x + vec.y*oth.y + vec.z*oth.z;
    }

    //Length of a vector
    double magnitude() const
    {
        return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    }

    //Magnitude is 1
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
