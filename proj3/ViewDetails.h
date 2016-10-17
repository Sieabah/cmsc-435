/**
 * Renderer.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * Implementation of class responsible for generation of image
 */

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

//Simple max function macro
#define MAX(a, b) ((a > b) ? a : b)

#include "Vector3D.h"
#include "Actor.h"
#include "Ray.h"

#include <limits>
#include <vector>
#include <iostream>
#include <utility>

class World;

#define PI 3.14159265358979323846

class ViewDetails {
public:
    //Default Constructor
    ViewDetails();

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

    /**
     * Getters
     */
    double FoV();
    unsigned int width();
    unsigned int height();
    Vector3D foreground(); //Not used anymore
    Vector3D background();

    /**
     * Variables from the book
     */
    Vector3D eye();
    Vector3D s();
    Vector3D d();
    Vector3D p(double t);
    Vector3D up();
    //Camera axis variables
    Vector3D w();
    Vector3D u();
    Vector3D v();
    double dist();
    double t();

    Vector3D spot(){
        return lookAt;
    }

    /**
     * Viewing pane variables
     */
    double top();
    double bottom();
    double right();
    double left();

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
    void Render(std::string outputFile, const std::vector<Actor *>* actors, World &world, bool printout=false);

    /**
     * AddLight
     * Add light to scene
     */
    void AddLight(Vector3D light);

    /**
     * CalcLighting
     * Calculate lighting of material, position, direction, and set of actors
     */
    Vector3D CalcLighting(Material material, Vector3D pos, Vector3D dir, Vector3D N, const std::vector<Actor*> *actors) const;

    /**
     * trace
     * Ray-Trace ray vector given set of actors
     */
    const Hit trace(Ray r, const std::vector<Actor*> *actors) const;

    const std::pair<unsigned int, unsigned int> resolution();

    double nearPlane(){ return hither; }
    double farPlane(){ return 500; }
private:
    //Lights
    std::vector<Vector3D> lights;

    //Color & Shading
    Vector3D backgroundColor;
    Vector3D fillColor;

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
