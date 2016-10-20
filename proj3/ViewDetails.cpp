/**
 * ViewDetails.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * Implementation of class responsible for generation of image
 */

#include "ViewDetails.h"

#include "Actor.h"
#include "Light.h"
#include "World.h"
#include <iostream>
#include <stdio.h>
#include "Color.h"

/**
 * Renderer
 * Default constructor
 */
ViewDetails::ViewDetails()
        : position(Eigen::Vector3d(0,0,0)), lookAt(Eigen::Vector3d(0,0,0)), upVector(Eigen::Vector3d(0,0,0)),
          fov(0), hither(0), resX(640),resY(480), backgroundColor(Eigen::Vector3d(0,0,0)),
          fillColor(Eigen::Vector3d(255,0,0)){};

/**
 * Helper function to print eigen vectors on a single line
 * @param vec
 */
void ViewDetails::printVectorToConsole(Eigen::Vector3d vec){
    std::cout << vec(0) << ", " << vec(1) << ", " << vec(2);
}

/**
 * PrintRenderInformation
 * Prints basic information about the renderer
 */
void ViewDetails::PrintRenderInformation()
{
    std::cout << "Background: "; printVectorToConsole(backgroundColor); std::cout << std::endl;
    std::cout << "Fill: "; printVectorToConsole(fillColor); std::cout << std::endl;

    std::cout << "Lights: " << std::endl;
    for (int i = 0; i < lights.size(); ++i) {
        std::cout << "\tLight " << i << ": "
                  << lights[i].position()(0) << ", "
                  << lights[i].position()(1) << ", "
                  << lights[i].position()(2) << std::endl;
    }

    std::cout << "Position: "; printVectorToConsole(position); std::cout << std::endl;
    std::cout << "LookAt: "; printVectorToConsole(lookAt); std::cout << std::endl;
    std::cout << "UpVector: "; printVectorToConsole(upVector); std::cout << std::endl;
    std::cout << "Angle: " << fov << std::endl;
    std::cout << "Hither: " << hither << std::endl;
    std::cout << "Resolution: " << resX << "x" << resY << std::endl;

    std::cout << "View Variables: " << std::endl;
    std::cout << "\te: "; printVectorToConsole(eye()); std::cout << std::endl;
    std::cout << "\tu: "; printVectorToConsole(u()); std::cout << std::endl;
    std::cout << "\tv: "; printVectorToConsole(v()); std::cout << std::endl;
    std::cout << "\tw: "; printVectorToConsole(w()); std::cout << std::endl;

    std::cout << "\tt: " << t() << std::endl;

    std::cout << "\ttop: " << top() << std::endl;
    std::cout << "\tbottom: " << bottom() << std::endl;
    std::cout << "\tleft: " << left() << std::endl;
    std::cout << "\tright: " << right() << std::endl;
}

/**
 * eye
 * Return camera position
 */
Eigen::Vector3d ViewDetails::eye() { return position; }

/**
 * background
 * Return background vector color
 */
Eigen::Vector3d ViewDetails::background() { return backgroundColor; }

/**
 * foreground
 * Return fill color vector color
 */
Eigen::Vector3d ViewDetails::foreground() { return fillColor; }

/**
 * height
 * Return Y resolution
 */
unsigned int ViewDetails::height() { return resY; }

/**
 * Return X resolution
 */
unsigned int ViewDetails::width() { return resX; }

/**
 * FoV
 * Return field of view
 */
double ViewDetails::FoV() { return fov; }

/**
 * Fill
 * Set fill color
 */
void ViewDetails::Fill(Eigen::Vector3d color) { fillColor = color; }

/**
 * Background
 * Set background color
 */
void ViewDetails::Background(double R, double G, double B) { backgroundColor = Eigen::Vector3d(R,G,B); }

/**
 * Resolution
 * Set X,Y Resolution
 */
void ViewDetails::Resolution(unsigned int x, unsigned int y) { resX = x; resY = y; }

/**
 * Hither
 * Set hither value
 */
void ViewDetails::Hither(double value) { hither = value; }

/**
 * Angle
 * Set fov or camera angle
 */
void ViewDetails::Angle(double value) { fov = value; }

/**
 * UpVec
 * Set the upwards vector
 */
void ViewDetails::UpVec(Eigen::Vector3d coord) { upVector = coord; }

/**
 * LookVec
 * Set the lookat vector
 */
void ViewDetails::LookVec(Eigen::Vector3d coord) { lookAt = coord; }

/**
 * Position
 * Set the camera position
 */
void ViewDetails::Position(Eigen::Vector3d coord) { position = coord; }

/**
 * t
 * return t-value to viewing pane
 */
double ViewDetails::t() { return tan(FoV() * PI/360); }

/**
 * top
 * Return distance to top side of viewing pane
 */
double ViewDetails::top() { return fabs(nearPlane()) * t() * (-1); }

/**
 * bottom
 * Return distance to bottom side of viewing pane
 */
double ViewDetails::bottom() { return top() * (-1); }

/**
 * right
 * Return distance to right side of viewing pane
 */
double ViewDetails::right() { return top() * (width() / height()); }

/**
 * left
 * Return distance to left side of viewing pane
 */
double ViewDetails::left() { return right() * (-1); }

/**
 * AddLight
 * Add light to render scene
 */
void ViewDetails::AddLight(Eigen::Vector3d light) { lights.push_back(Light(light)); }

/**
 * Get look at vector
 * @return
 */
Eigen::Vector3d ViewDetails::look() { return Eigen::Vector3d(spot() - eye()); }

/**
 * Get right vector
 * @return
 */
Eigen::Vector3d ViewDetails::rightVec() { return look().cross(upPoint()); }

/**
 * Get up vector
 * @return
 */
Eigen::Vector3d ViewDetails::up() { return rightVec().cross(look()); }

/**
 * Get up coordinate
 * @return
 */
Eigen::Vector3d ViewDetails::upPoint() { return upVector; }

/**
 * w Vector (look, -w is towards look at) Normalized
 * @return
 */
Eigen::Vector3d ViewDetails::w() { return look().normalized() * -1; }

/**
 * u Vector (right vector) Normalized
 * @return
 */
Eigen::Vector3d ViewDetails::u() { return up().cross(w()).normalized(); }

/**
 * v Vector (up vector) Normalized
 * @return
 */
Eigen::Vector3d ViewDetails::v() { return w().cross(u()); }

/**
 * Look at coordinate
 * @return
 */
Eigen::Vector3d ViewDetails::spot() { return lookAt; }

/**
 * Get near clipping distance
 * @return
 */
double ViewDetails::nearPlane() { return hither; }

/**
 * Get far clipping distance
 * @return
 */
double ViewDetails::farPlane() { return nearPlane() * 1000; }

/**
 * Get pointer to light
 * @param index Light index
 * @return
 */
const Light *ViewDetails::getLight(int index) {
    if (index >= lights.size() || index < 0)
        return NULL;

    return &lights[index];
}

/**
 * Get all lights
 * @return
 */
const std::vector<Light> *ViewDetails::getLights() { return &lights; }
