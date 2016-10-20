/**
 * ViewDetails.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * Implementation of class responsible for generation of image
 */

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include <vector>
#include <Eigen/Dense>

#include "Light.h"

class Actor;
class World;

#define PI 3.14159265358979323846

class ViewDetails {
public:
    /**
     * Default constructor
     * @return
     */
    ViewDetails();

    /**
     * Set camera position coordinate
     * @param coord
     */
    void Position(Eigen::Vector3d coord);

    /**
     * Set look at coordinate
     * @param coord
     */
    void LookVec(Eigen::Vector3d coord);

    /**
     * Set up coordinate
     * @param coord
     */
    void UpVec(Eigen::Vector3d coord);

    /**
     * Set Field of View Angle
     * @param value
     */
    void Angle(double value);

    /**
     * Set near clipping field hither
     * @param value
     */
    void Hither(double value);

    /**
     * Set image resolution
     * @param x
     * @param y
     */
    void Resolution(unsigned int x, unsigned int y);

    /**
     * Set background color [0, 1]
     * @param R Red channel
     * @param G Green channel
     * @param B Blue channel
     */
    void Background(double R, double G, double B);

    /**
     * Set fill color
     * @param color
     */
    void Fill(Eigen::Vector3d color);

    /**
     * Get field of view
     * @return
     */
    double FoV();

    /**
     * Get image width
     * @return
     */
    unsigned int width();

    /**
     * Get image height
     * @return
     */
    unsigned int height();

    /**
     * Get image foreground
     * @return
     */
    Eigen::Vector3d foreground();

    /**
     * Get Image background
     */
    Eigen::Vector3d background();

    /**
     * Variables from the book
     */
    /**
     * Get eye coordinate
     * @return
     */
    Eigen::Vector3d eye();

    /**
     * Get look at vector
     * @return
     */
    Eigen::Vector3d look();

    /**
     * Get right vector
     * @return
     */
    Eigen::Vector3d rightVec();

    /**
     * Get up vector
     * @return
     */
    Eigen::Vector3d up();

    /**
     * Get up coordinate
     * @return
     */
    Eigen::Vector3d upPoint();

    /**
     * w Vector (look, -w is towards look at) Normalized
     * @return
     */
    Eigen::Vector3d w();

    /**
     * u Vector (right vector) Normalized
     * @return
     */
    Eigen::Vector3d u();

    /**
     * v Vector (up vector) Normalized
     * @return
     */
    Eigen::Vector3d v();

    /**
     * Look at coordinate
     * @return
     */
    Eigen::Vector3d spot();

    /**
     * Get t variable (from book)
     * @return
     */
    double t();

    /**
     * Get value for top of frustum
     * @return
     */
    double top();

    /**
     * Get value for bottom of frustum
     * @return
     */
    double bottom();

    /**
     * Get value for right of frustum
     * @return
     */
    double right();

    /**
     * Get value for left of frustum
     * @return
     */
    double left();

    /**
     * Get near clipping distance
     * @return
     */
    double nearPlane();

    /**
     * Get far clipping distance
     * @return
     */
    double farPlane();

    /**
     * PrintRenderInformation
     * Prints basic render information
     */
    void PrintRenderInformation();

    /**
     * AddLight
     * Add light to scene
     */
    void AddLight(Eigen::Vector3d light);

    /**
     * Get pointer to light
     * @param index Light index
     * @return
     */
    const Light *getLight(int index);

    /**
     * Get all lights
     * @return
     */
    const std::vector<Light> *getLights();

private:
    /**
     * Store all lights in scene
     */
    std::vector<Light> lights;

    /**
     * Background color
     */
    Eigen::Vector3d backgroundColor;

    /**
     * Foreground color
     */
    Eigen::Vector3d fillColor;

    /**
     * Position coordinate
     */
    Eigen::Vector3d position;

    /**
     * Look at coordinate
     */
    Eigen::Vector3d lookAt;

    /**
     * Up coordinate
     */
    Eigen::Vector3d upVector;

    /**
     * Field of view in degrees
     */
    double fov;

    /**
     * Hither
     */
    double hither;

    //Resolution
    unsigned int resX;
    unsigned int resY;

    /**
     * Helper to print eigen vectors to console on a single line
     * @param vec
     */
    void printVectorToConsole(Eigen::Vector3d vec);
};


#endif //RAYTRACER_RENDERER_H
