/**
 * World.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * World header definition
 */

#ifndef RAYTRACER_WORLD_H
#define RAYTRACER_WORLD_H

#include "Polygon.h"
#include "Renderer.h"
#include "Vector3D.h"
#include <string>
#include <vector>

class World {
public:
    //Default constructor
    World(): camera(Renderer()){};

    /**
     * getRenderer
     * Returns pointer to renderer
     */
    Renderer* getRenderer(){ return &camera; }

    /**
     * AddPoly
     * Adds polygon to world
     */
    void AddPoly(const Polygon &poly){ polygons.push_back(poly); }

    /**
     * PrintWorldInformation
     * Prints information about renderer and world
     */
    void PrintWorldInformation();

    /**
     * GenerateWorldFromNFF
     * Generate a world object from a NFF file
     */
    static World GenerateWorldFromNFF(std::string filepath);

    void Render(std::string outputFile, bool printout=false)
    {
        //Sorry didn't have time to implement this
        printout = false;
        camera.Render(outputFile, Polys(), printout);
    }

    std::vector<Polygon>* Polys(){ return &polygons; }
private:
    std::vector<Polygon> polygons;
    Renderer camera;
};


#endif //RAYTRACER_WORLD_H
