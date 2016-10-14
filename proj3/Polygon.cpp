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
 * addVert
 * Add vertex to polygon
 */
void Polygon::addVert(Vector3D coord) {
    vertices.push_back(vertex(coord));
}

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
 * intersect
 * Detect if ray intersects within poly
 */
const Hit Polygon::intersect(const Ray &ray) const {
    //Find exact distance of the ray to the poly
    double distance = (vecNormal - Vector3D::dot(norm, ray.pos())) / Vector3D::dot(norm, ray.dir());

    //If distance is outside of ray plane, no hit
    if(distance < ray.near() || distance > ray.far())
        return Hit();

    //Determine point
    Vector3D point = ray.pos() + ray.dir() * distance;

    //Find tangent and bitangent
    double pointTangent = Vector3D::dot(point, tangent);
    double pointBitangent = Vector3D::dot(point, bitangent);

    bool inside = false;

    //Iterate over all vertices, determining if ray is inside the two points
    std::vector<vertex>::const_iterator vert0, vert1;
    for(vert1 = vertices.begin(), vert0 = vert1++; vert1 != vertices.end(); vert0 = vert1, ++vert1)
    {
        double point1 = vert1->bitangent - pointBitangent;
        double point2 = pointBitangent - vert0->bitangent;

        if((point1 > 0) != (point2 < 0))
        {
            double q_tangent = (point1 * vert0->tangent + point2 * vert1->tangent)/
                    (vert1->bitangent - vert0->bitangent);

            if(q_tangent > pointTangent)
                inside = !inside;
        }
    }

    //If determined to be inside, create hit object
    if (inside) return Hit(this, distance, ray);

    //Ohterwise no hit generated
    return Hit();
}

/**
 * addMaterial
 * Add material to polygon
 */
void Polygon::addMaterial(Material &mat) {
    material = mat;
}