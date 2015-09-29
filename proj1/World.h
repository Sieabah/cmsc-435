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
    World();

    /**
     * Return pointer to renderer
     */
    Renderer * getRenderer();

    /**
     * AddPoly
     * Add polygon to world
     */
    void AddPoly(const Polygon &poly);

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

    /**
     * Render
     * outputFile - filename to write to
     */
    void Render(std::string outputFile, bool printout=false);

    /**
     * Polys
     * Return pointer to polygons vector
     */
    std::vector<Polygon> * Polys();
private:
    //Polygons in world
    std::vector<Polygon> polygons;
    //Camera
    Renderer camera;
};


#endif //RAYTRACER_WORLD_H
