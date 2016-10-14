/**
 * Hit.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * Intersection or hit of ray with Actor instance
 */

#ifndef RAYTRACER_HIT_H
#define RAYTRACER_HIT_H
#define BOUNCE_LIMIT 5

#include "Vector3D.h"

class World;
class Actor;
class Ray;

class Hit {
public:
    /**
     * Default constructor
     */
    Hit(): length(INFINITY), actor(NULL){};

    /**
     * Constructor with initializers
     */
    Hit(const Actor *other, double dist, const Ray& r, int rec=0);

    /**
     * color
     * Determine color of location hit
     */
    const Vector3D color(World& world) const;

    /**
     * operator<
     * Less than operator overloading
     */
    bool operator<(const Hit &oth);

    /**
     * len
     * Returns length of ray
     */
    double len() const {return length;}
private:
    //Distance from ray origin
    double length;

    //Color calculation (reflection bounce limit)
    int recursion;

    //Ray information
    Vector3D pos, dir, normal;

    //Pointer to actor
    const Actor *actor;
};


#endif //RAYTRACER_HIT_H
