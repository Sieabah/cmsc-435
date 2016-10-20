/**
 * Polygon.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * n-sided polygon
 */

#include "Polygon.h"

/**
 * finalize
 * Precalculate values to detect if ray intersects poly
 */
void Polygon::finalize() {
    Eigen::Vector3d e1(vertices[1].pos - vertices[0].pos);
    Eigen::Vector3d e2(vertices[2].pos - vertices[1].pos);

    norm = e1.cross(e2).normalized();

    tangent = (vertices[0].pos - vertices[2].pos).normalized();
    bitangent = norm.cross(tangent);

    //Iterate over vertices and calculate tangent and bitagents for each
    for(std::vector<vertex>::iterator it = vertices.begin(); it != vertices.end(); it++)
    {
        it->tangent = it->pos.dot(tangent);
        it->bitangent = it->pos.dot(bitangent);
    }

    vecNormal = vertices[0].pos.dot(norm);
}