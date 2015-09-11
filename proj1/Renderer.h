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
    Renderer();

    /**
     * Setters
     */
    void Position(Vector3D coord);
    void LookVec(Vector3D coord);
    void UpVec(Vector3D coord);
    void Angle(double value);
    void Hither(double value);
    void Resolution(unsigned int x, unsigned int y);
    void Background(double R, double G, double B);
    void Fill(Vector3D color);
    void Shader(Shading newShader);

    /**
     * Getters
     */
    double FoV();
    unsigned int width();
    unsigned int height();
    Vector3D foreground();
    Vector3D background();

    /**
     * Variables from the book
     */
    Vector3D eye();
    Vector3D s();
    Vector3D d();
    Vector3D p(double t);
    Vector3D up();

    /**
     * PrintRenderInformation
     * Prints basic render information
     */
    void PrintRenderInformation();

    /**
     * Render
     * outputFile - The output file to write pixel data to
     * polys - All the polygons in the world to ray-trace
     * printout - Print to console or write to file
     */
    void Render(std::string outputFile, std::vector<Polygon>* polys, bool printout=false);

private:

    //Color & Shading
    Vector3D backgroundColor;
    Vector3D fillColor;
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
