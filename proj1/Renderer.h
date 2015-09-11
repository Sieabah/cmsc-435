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

#define PI 3.14159265358979323846

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
      fov(0), hither(0), resX(640),resY(480),
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
     * Update up vector
     */
    void UpVec(Vector3D coord) { upVector = coord; };
    /**
     * Angle
     * Update camera fov
     */
    void Angle(double value) { fov = value; };
    /**
     * Hither
     * Update camera hither
     */
    void Hither(double value) { hither = value; };
    /**
     * Resolution
     * Update camera resolution
     */
    void Resolution(unsigned int x, unsigned int y) { resX = x; resY = y; };

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

    double FoV(){ return fov; }
    unsigned int width(){ return resX; }
    unsigned int height(){ return resY; }

    Vector3D foreground(){ return fill_color; }
    Vector3D background(){ return background_color; }

    /**
     * PrintRenderInformation
     * Prints basic render information
     */
    void PrintRenderInformation();

    Vector3D eye(){ return position; }

    Vector3D s(){ return lookAt; }

    Vector3D d(){ return s() - eye(); }

    Vector3D p(double t){ return eye() + (d() * t); }

    Vector3D up()
    {
        return upVector;
    }

    double projectionFromCenter()
    {
        double lensHalfAngle = fov /2;
        double deg = PI/180;

        return (d().magnitude() * sinf((float) (lensHalfAngle*deg)))/sinf((float) ((90-lensHalfAngle)*deg));
    };

    void Render(std::string outputFile, std::vector<Polygon>* polys, bool printout=false);

private:

    //Color & Shading
    Vector3D background_color;
    Vector3D fill_color;
    Shading shader;

    //Position and Direction
    Vector3D position;
    Vector3D lookAt;
    Vector3D upVector;
    double fov;
    double hither;

    //Resolution
    unsigned int resX;
    unsigned int resY;
};


#endif //RAYTRACER_RENDERER_H
