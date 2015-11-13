/**
 * Vector3D.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * Simple 3 dimensional vector
 */

#include "Vector3D.h"


/**
 * Default constructor
 */
Vector3D::Vector3D(double ix, double iy , double iz)
    : x(ix), y(iy), z(iz) { name = ""; stringify(); }

/**
 * Copy Constructor
 */
Vector3D::Vector3D(const Vector3D &other) : x(other.x), y(other.y), z(other.z) { name = ""; stringify(); }

/**
 * Add two vectors together
 */
Vector3D Vector3D::operator+(const Vector3D &other) const
{
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

/**
 * Subtract two vectors from one another
 */
Vector3D Vector3D::operator-(const Vector3D &other) const
{
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

/**
 * Multiply vector by a double
 */
Vector3D Vector3D::operator*(const double &other) const
{
    return Vector3D(x * other, y * other, z * other);
}

/**
 * Multiply vector by an integer
 */
Vector3D Vector3D::operator*(const int &other) const
{
    return Vector3D(x * other, y * other, z * other);
}

/**
 * Equality operator
 */
bool Vector3D::operator==(const Vector3D &other) const
{
    return !(x != other.x || y != other.y || z != other.z);
}

/**
 * Less than operator
 */
bool Vector3D::operator<(const Vector3D &other) const {
    return magnitude() < other.magnitude();
}

/**
 * Multiplication operator
 */
double Vector3D::operator*(const Vector3D &other) const {
    return Vector3D::dot(*this, other);
}

/**
 * power operator
 */
double Vector3D::operator^(int value) const {
    if(value != 2)
        throw "Cannot raise vector to power given";

    return Vector3D::dot(*this, *this);
}

/**
 * dot product
 */
double Vector3D::dot(const Vector3D &vec, const Vector3D &oth)
{
    return vec.x*oth.x + vec.y*oth.y + vec.z*oth.z;
}

/**
 * cross product
 */
Vector3D Vector3D::cross(const Vector3D &vec, const Vector3D &oth)
{
    return Vector3D(vec.y*oth.z-vec.z*oth.y, vec.z*oth.x-vec.x*oth.z,vec.x*oth.y - vec.y*oth.x);
}

/**
 * vector angle
 */
float Vector3D::angle(const Vector3D &oth)
{
    return acosf((float)((*this*oth)/this->magnitude() / oth.magnitude()));
}

/**
 * vector length
 */
double Vector3D::magnitude() const
{
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

/**
 * unit vector
 */
Vector3D Vector3D::unit()
{
    double mag = magnitude();
    return Vector3D(x/mag, y/mag, z/mag);
}

Vector3D Vector3D::midpoint(const Vector3D &vec, const Vector3D &oth) {
    return Vector3D((vec.x+oth.x)/2, (vec.y+oth.y)/2, (vec.z+oth.z)/2);
}
