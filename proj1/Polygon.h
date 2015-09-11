/**
 * Polygon.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * Basic n-sided polygon
 */

#ifndef RAYTRACER_POLYGON_H
#define RAYTRACER_POLYGON_H

#include <vector>
#include "Vector3D.h"

class Polygon {
public:
    Polygon(){}

    /**
     * addVert
     * Add vertex to polygon
     */
    void addVert(Vector3D coord)
    {
        vertices.push_back(coord);
    }

    virtual bool intersects(Vector3D ray, Vector3D direction)
    {
        Vector3D c1 = vertices[0] - vertices[1];
        Vector3D c2 = vertices[0] - vertices[2];
        Vector3D c3 = direction;
        Vector3D c4 = vertices[0] - ray;

        double A = c1.x, B = c1.y, C = c1.z;
        double D = c2.x, E = c2.y, F = c2.z;
        double G = c3.x, H = c3.y, I = c3.z;
        double J = c4.x, K = c4.y, L = c4.z;

        double M = A*(E*I - H*F) + B*(G*F - D*I) + C*(D*H - E*G);

        double t = -(F*(A*K - J*B) + E*(J*C - A*L) + D*(B*L - K*C)) / M;
        if(t < 0.0f) return false;

        double g = (I*(A*K - J*B) + H*(J*C - A*L) + G*(B*L - K*C)) / M;
        if(g < 0.0f || g > 1.0f) return false;

        double b = (J*(E*I - H*F) + K*(G*F - D*I) + L*(D*H - E*G)) / M;
        return !(b < 0.0f || b > 1.0f - g);

    }
private:
    std::vector<Vector3D> vertices;
};


#endif //RAYTRACER_POLYGON_H
