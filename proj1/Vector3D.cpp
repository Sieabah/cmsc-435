/**
 * Vector3D.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * Vector implementation
 */

#include "Vector3D.h"

//Default contructor
Vector3D::Vector3D(double ix, double iy , double iz)
    : x(ix), y(iy), z(iz) {}

/**
 * Add two vectors together
 */
Vector3D Vector3D::operator+(const Vector3D &other)
{
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

/**
 * Subtract two vectors from one another
 */
Vector3D Vector3D::operator-(const Vector3D &other)
{
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

/**
 * Multiply vector by a double
 */
Vector3D Vector3D::operator*(const double &other)
{
    return Vector3D(x * other, y * other, z * other);
}

/**
 * Multiply vector by an integer
 */
Vector3D Vector3D::operator*(const int &other)
{
    return Vector3D(x * other, y * other, z * other);
}

bool Vector3D::operator==(const Vector3D &other)
{
    return !(x != other.x || y != other.y || z != other.z);
}

bool Vector3D::operator<(const Vector3D &other) {
    return magnitude() < other.magnitude();
}
