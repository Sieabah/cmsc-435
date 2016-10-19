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

#include "Vector3D.h"
#include "Actor.h"
#include "Ray.h"
#include "Light.h"

#include <limits>
#include <vector>
#include <iostream>
#include <utility>
#include <Eigen/Dense>

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
    Eigen::Vector3d eye();
    Eigen::Vector3d look(){ return Eigen::Vector3d(spot() - eye()); }
    Eigen::Vector3d rightVec(){ return look().cross(upPoint()); }
    Eigen::Vector3d up(){ return rightVec().cross(look()); }
    Eigen::Vector3d upPoint(){ return Eigen::Vector3d(upVector.x, upVector.y, upVector.z); }
    Eigen::Vector3d w(){ return look().normalized() * -1; }
    Eigen::Vector3d u(){ return up().cross(w()).normalized(); }
    Eigen::Vector3d v(){ return w().cross(u()); }

    Eigen::Vector3d spot(){
        return Eigen::Vector3d(lookAt.x, lookAt.y, lookAt.z);
    }

    /**
     * Viewing pane variables
     */
    double t();
    double top();
    double bottom();
    double right();
    double left();

    double nearPlane(){ return hither; }
    double farPlane(){ return nearPlane()*1000; }

    /**
     * PrintRenderInformation
     * Prints basic render information
     */
    void PrintRenderInformation();

    /**
     * AddLight
     * Add light to scene
     */
    void AddLight(Vector3D light);

    const Light *getLight(int index){
        if(index >= lights.size() || index < 0)
            return NULL;

        return &lights[index];
    }

    const std::vector<Light> *getLights(){
        return &lights;
    }
private:
    //Lights
    std::vector<Light> lights;

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
