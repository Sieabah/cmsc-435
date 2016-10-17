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

#include "ViewDetails.h"
#include "Hit.h"
#include "Ray.h"

#include <string>
#include <vector>

class Polygon;

class World {
public:
    //Default constructor
    World();

    //Virtual destructor
    virtual ~World()
    {
        //For all actors
        while(!actors.empty()){
            //Delete
            delete actors.back();
            actors.pop_back();
        }
    }

    /**
     * Return pointer to renderer
     */
    ViewDetails * getRenderer();

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
    ViewDetails camera;

    static std::vector<Polygon*> generatePolys(std::ifstream &file, std::string &line, int &count);
};


#endif //RAYTRACER_WORLD_H
