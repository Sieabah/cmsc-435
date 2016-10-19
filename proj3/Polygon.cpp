/**
 * Polygon.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * n-sided polygon
 */

#include "Polygon.h"

/**
 * finalize
 * Precalculate values to detect if ray intersects poly
 */
void Polygon::finalize() {
    Vector3D e1 = vertices[1].pos - vertices[0].pos;
    Vector3D e2 = vertices[2].pos - vertices[1].pos;

    norm = Vector3D::cross(e1, e2).unit();
    tangent = (vertices.front().pos - vertices.back().pos).unit();
    bitangent = Vector3D::cross(norm, tangent);

    //Iterate over vertices and calculate tangent and bitagents for each
    for(std::vector<vertex>::iterator it = vertices.begin(); it != vertices.end(); it++)
    {
        it->tangent = Vector3D::dot(it->pos, tangent);
        it->bitangent = Vector3D::dot(it->pos, bitangent);
    }

    vecNormal = Vector3D::dot(vertices.front().pos, norm);
}

/**
 * normal
 * Returns normal vector from other vector
 */
Vector3D Polygon::normal(const Vector3D &vec) const {
    return norm;
}

/**
 * addMaterial
 * Add material to polygon
 */
void Polygon::addMaterial(Material &mat) {
    material = mat;
}