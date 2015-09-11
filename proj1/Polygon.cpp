//
// Created by Christopher on 9/5/2015.
//

#include "Polygon.h"

/**
 * addVert
 * Add vertex to polygon
 */
void Polygon::addVert(Vector3D coord) {
    vertices.push_back(coord);
}

/**
 * intersect
 * Determine if ray intersects with polygon
 */
bool Polygon::intersects(Vector3D ray, Vector3D direction) {
    //Setup variables for for easier use
    Vector3D c1 = vertices[0] - vertices[1];
    Vector3D c2 = vertices[0] - vertices[2];
    Vector3D c3 = direction;
    Vector3D c4 = vertices[0] - ray;

    //Assign A-L based on the matrix given in book
    double A = c1.x, B = c1.y, C = c1.z;
    double D = c2.x, E = c2.y, F = c2.z;
    double G = c3.x, H = c3.y, I = c3.z;
    double J = c4.x, K = c4.y, L = c4.z;

    //Precalculate parts of the equations
    double EIHF = (E*I - H*F);
    double GFDI = (G*F - D*I);
    double DHEG = (D*H - E*G);

    double AKJB = (A*K - J*B);
    double JCAL = (J*C - A*L);
    double BLKC = (B*L - K*C);

    //Assign M based on book
    double M = A*EIHF + B*GFDI + C*DHEG;

    //Ray-trace algorithm

    //Calculate t
    double t = -(F*AKJB + E*JCAL + D*BLKC) / M;
    if(t < 0.0f) return false;

    //Calculate gamma
    double g = (I*AKJB + H*JCAL + G*BLKC) / M;
    if(g < 0.0f || g > 1.0f) return false;

    //Calculate beta
    double b = (J*EIHF + K*GFDI + L*DHEG) / M;
    return !(b < 0.0f || b > 1.0f - g);
}