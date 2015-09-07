/**
 * Renderer.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * Renderer header definition
 */

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include "Vector3D.h"
#include "Polygon.h"
#include <vector>
#include <iostream>

/**
 * Shader struct
 * Holds basic shader information
 */
struct Shading{
    double Kd;
    double Ks;
    double Shine;
    double T;
    double index_of_refraction;
};

class Renderer {
public:
    //Default Constructor
    Renderer()
    : position(Vector3D()), lookAt(Vector3D()), upVector(Vector3D()),
      angle(0), hither(0), resX(640),resY(480),
      background_color(Vector3D(0,0,0)), fill_color(Vector3D(255,0,0)){};

    /**
     * Position
     * Update position vector
     */
    void Position(Vector3D coord) { position = coord; };
    /**
     * LookVec
     * Update look-at vector
     */
    void LookVec(Vector3D coord) { lookAt = coord; };
    /**
     * UpVec
     * Update up vecotr
     */
    void UpVec(Vector3D coord) { upVector = coord; };
    /**
     * Angle
     * Update camera angle
     */
    void Angle(double value) { angle = value; };
    /**
     * Hither
     * Update camera hither
     */
    void Hither(double value) { hither = value; };
    /**
     * Resolution
     * Update camera resolution
     */
    void Resolution(int x, int y) { resX = x; resY = y; };

    /**
     * Background
     * Update background color
     */
    void Background(double R, double G, double B){ background_color = Vector3D(R,G,B); };
    /**
     * Fill
     * Update fill color
     */
    void Fill(Vector3D color){ fill_color = color; };
    /**
     * Shader
     * Update shader
     */
    void Shader(Shading new_shader){ shader = new_shader; };

    /**
     * PrintRenderInformation
     * Prints basic render information
     */
    void PrintRenderInformation();

    Vector3D eye(){ return position; }

    Vector3D s(){ return lookAt; }

    Vector3D d(){ return s() - eye(); }

    Vector3D p(double t){ return eye() + (d() * t); }

private:

    //Color & Shading
    Vector3D background_color;
    Vector3D fill_color;
    Shading shader;

    //Position and Direction
    Vector3D position;
    Vector3D lookAt;
    Vector3D upVector;
    double angle;
    double hither;

    //Resolution
    int resX;
    int resY;

};


#endif //RAYTRACER_RENDERER_H
