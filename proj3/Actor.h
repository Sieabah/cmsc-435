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

#include <vector>

#include "Hit.h"
#include "Material.h"
#include <Eigen/Dense>

class Ray;
class World;

/**
 * vertex
 * point in space
 */
class vertex {
public:
    vertex(){}
    virtual ~vertex(){
        if(camera_pos){
            delete camera_pos;
            camera_pos = nullptr;
        }
    }

    //3D point in space
    Vector3D pos;

    //3D point in space
    Eigen::Vector4d *camera_pos = nullptr;
    //Tangent and bitangent of this point
    double tangent, bitangent;

    /**
     * vertex
     * constructor
     */
    vertex(const Vector3D &vec){pos = vec;}
};

class Actor {
public:

    /**
     * addVert
     * Add vertex to polygon
     */
    void addVert(Vector3D coord) {
        vertices.push_back(vertex(coord));
    }

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

    std::vector<vertex>* getVerticies(){
        return &vertices;
    }
protected:
    //Material of actor
    Material material;

    //Vertices of the polygon
    std::vector<vertex> vertices;
};


#endif //RAYTRACER_ACTOR_H
