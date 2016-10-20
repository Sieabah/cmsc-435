/**
 * Actor.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * Abstract definition of an entity in the world
 */

#ifndef RAYTRACER_ACTOR_H
#define RAYTRACER_ACTOR_H

#include <vector>
#include <Eigen/Dense>

#include "Material.h"

class Ray;
class World;

/**
 * vertex
 * point in space
 */
class vertex {
public:
    vertex():camera_pos(NULL){}
    virtual ~vertex(){}

    //3D point in space
    Eigen::Vector3d pos;

    //3D point in space
    Eigen::Vector4d *camera_pos;
    //Tangent and bitangent of this point
    double tangent, bitangent;

    vertex(const Eigen::Vector3d &vec){
        pos = vec;
    }
};

class Actor {
public:

    /**
     * addVert
     * Add vertex to polygon
     */
    void addVert(Eigen::Vector3d coord) {
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
