/**
 * World.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * World header definition
 */

#ifndef RAYTRACER_WORLD_H
#define RAYTRACER_WORLD_H

#include "Vector3D.h"

#include "Renderer.h"
#include "Hit.h"
#include "Ray.h"

#include <string>
#include <vector>

class World {
public:
    //Default constructor
    World();

    //Virtual destructor
    virtual ~World()
    {
        //For all actors
        for(int i = 0; i < actors.size(); i++)
            //Delete them
            delete &actors[i];
    }

    /**
     * Return pointer to renderer
     */
    Renderer * getRenderer();

    /**
     * AddPoly
     * Add polygon to world
     */
    void AddActor(Actor *actor);

    /**
     * AddLight
     * Add lights to the world
     */
    void AddLight(const Vector3D &light);

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
    const std::vector<Actor*> *Actors() const;
private:
    //Polygons in world
    std::vector<Actor*> actors;
    //Camera
    Renderer camera;
};


#endif //RAYTRACER_WORLD_H
