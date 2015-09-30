/**
 * Hit.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * Intersection or hit of ray with Actor instance
 */

#include "Hit.h"

#include "Actor.h"
#include "World.h"

/**
 * color
 * Determine color of hit on actor object
 */
const Vector3D Hit::color(World &world) const {
    //If we hit an actor
    if(actor)
    {
        Vector3D color = world.getRenderer()->CalcLighting(actor->Texture(), pos, dir, normal, world.Actors());

        //If reflectivity > 0, get bounced items within recursive limit
        if(actor->Texture().shader.Ks > 0.0 && recursion < BOUNCE_LIMIT)
        {
            //Find reflection vector
            Vector3D reflection = dir - (normal * 2.0f * Vector3D::dot(dir, normal));

            //Get next reflection hit
            Hit next = world.getRenderer()->trace(Ray(pos,reflection), world.Actors());

            //Assign next recursion
            next.recursion = recursion + 1;

            //Add up color from recursive calculation
            color = color + (next.color(world) * actor->Texture().shader.Ks);
        }

        return color;
    }
    else
    {
        //Give background color otherwise
        return world.getRenderer()->background();
    }
}

/**
 * Constructor with initializer
 */
Hit::Hit(const Actor *other, double dist, const Ray &r, int rec)
: actor(other), length(dist),recursion(rec), dir(r.dir()){
    //Get hit position
    pos = r.pos() + (r.dir() * dist);

    //Get normal vector based on other actor
    normal = other->normal(pos).unit();
}

/**
 * operator<
 * less than operator overloaded
 */
bool Hit::operator<(const Hit &oth) {
    return len() < oth.len();
}