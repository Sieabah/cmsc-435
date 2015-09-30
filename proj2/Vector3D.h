/**
 * Vector3D.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * Simple 3 dimensional vector
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

    //Copy Constructor
    Vector3D(const Vector3D &other);

    //Vector values
    double x;
    double y;
    double z;

    //Math operator overrides
    Vector3D operator+(const Vector3D &other) const;
    Vector3D operator-(const Vector3D &other) const;
    Vector3D operator*(const double &other) const;
    Vector3D operator*(const int &other) const;
    double operator*(const Vector3D &other) const;
    double operator^(int value) const;
    bool operator==(const Vector3D &other) const;
    bool operator<(const Vector3D &other) const;

    /**
     * dot
     * Perform dot product on two given vectors
     */
    static double dot(const Vector3D &vec, const Vector3D &oth);

    /**
     * cross
     * Perform cross product on two given vectors
     */
    static Vector3D cross(const Vector3D &vec, const Vector3D &oth);

    /**
     * angle
     * Determine angle between two vectors
     */
    float angle(const Vector3D &oth);

    /**
     * magnitude
     * Find length/magnitude of the vector
     */
    double magnitude() const;

    /**
     * unit
     * Normalize the vector
     */
    Vector3D unit();

    //Printable interface (I can't get this working with it being in the header)
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& vec)
    {
        os << vec.x << ',' << vec.y << ',' << vec.z;
        return os;
    }
};

#endif //RAYTRACER_VECTOR3D_H
