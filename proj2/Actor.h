/**
 * Actor.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * Abstract definition of an entity in the world
 */
#ifndef RAYTRACER_ACTOR_H
#define RAYTRACER_ACTOR_H

#include "Hit.h"
#include "Material.h"

class Ray;
class World;

class Actor {
public:
    /**
     * Actor
     * Default constructor
     */
    Actor(){};

    /**
     * Constructor initializes with material
     */
    Actor(Material &mat):material(mat){};

    /**
     * Virtual destructor
     */
    virtual ~Actor(){};

    /**
     * intersect
     * abstract function to determine if actor has been hit by given ray
     */
    virtual const Hit intersect(const Ray &ray) const = 0;

    /**
     * normal
     * Return normal vector given another vector
     */
    virtual Vector3D normal(const Vector3D &vec) const = 0;

    /**
     * Texture
     * returns material texture of actor
     */
    Material Texture() const {return material;}
protected:
    //Material of actor
    Material material;
};


#endif //RAYTRACER_ACTOR_H
