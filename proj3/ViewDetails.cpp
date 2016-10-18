/**
 * Renderer.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * Implementation of class responsible for generation of image
 */

#include "ViewDetails.h"
#include "World.h"
#include <iostream>
#include <stdio.h>
#include "Color.h"

/**
 * Renderer
 * Default constructor
 */
ViewDetails::ViewDetails(): position(Vector3D()), lookAt(Vector3D()), upVector(Vector3D()),
                      fov(0), hither(0), resX(640),resY(480),
                      backgroundColor(Vector3D(0,0,0)), fillColor(Vector3D(255,0,0)){};

/**
 * PrintRenderInformation
 * Prints basic information about the renderer
 */
void ViewDetails::PrintRenderInformation()
{
    std::cout << "Background: " << backgroundColor << std::endl;
    std::cout << "Fill: " << fillColor << std::endl;

    std::cout << "Lights: " << std::endl;
    for (int i = 0; i < lights.size(); ++i) {
        std::cout << "\tLight " << i << ": " << lights[i] << std::endl;
    }

    std::cout << "Position: " << position << std::endl;
    std::cout << "LookAt: " << lookAt << std::endl;
    std::cout << "UpVector: " << upVector << std::endl;
    std::cout << "Angle: " << fov << std::endl;
    std::cout << "Hither: " << hither << std::endl;
    std::cout << "Resolution: " << resX << "x" << resY << std::endl;

    std::cout << "View Variables: " << std::endl;
    std::cout << "\te: " << eye() << std::endl;
    std::cout << "\ts: " << s() << std::endl;
    std::cout << "\td: " << d() << std::endl;

    std::cout << "\tp(0): " << p(0) << std::endl;
    std::cout << "\tp(1): " << p(1) << std::endl;

    std::cout << "\tw: " << w() << std::endl;
    std::cout << "\tu: " << u() << std::endl;
    std::cout << "\tv: " << v() << std::endl;

    std::cout << "\tt: " << t() << std::endl;

    std::cout << "\ttop: " << top() << std::endl;
    std::cout << "\tbottom: " << bottom() << std::endl;
    std::cout << "\tleft: " << left() << std::endl;
    std::cout << "\tright: " << right() << std::endl;
}

/**
 * Render
 * render image to screen with given actors and world
 */
void ViewDetails::Render(std::string outputFile, const std::vector<Actor*>* actors, World &world, bool printout) {

    //Data array
    unsigned char pixels[height()][width()][3];

    //For each row
    for(int hPixel =0; hPixel <height(); ++hPixel) {

        if (hPixel % (height() / 25) == 0)
            printf("%d%%...", (100* hPixel)/height());

        //For each column
        for(int wPixel =0; wPixel <width(); ++wPixel) {
            // trace new ray (Equations from book)
            double us = left() + (right() - left()) * (wPixel +0.5)/width();
            double vs = top() + (bottom() - top()) * (hPixel +0.5)/height();

            //Ray direction
            Vector3D dir = w() * -dist() + u() * us + v() * vs;

            //Generate ray
            Ray ray(eye(), dir, hither / dist());

            //Figure color of the ray
            Color color = Color(trace(ray, actors).color(world));

            // assign color
            pixels[hPixel][wPixel][0] = color.R;
            pixels[hPixel][wPixel][1] = color.G;
            pixels[hPixel][wPixel][2] = color.B;
        }
    }

    //Given by prompt
    //Open file
    FILE *f = fopen(outputFile.c_str(),"wb");

    //Protect against not being able to write correctly
    if(f == NULL){
        std::cout << "Could not open file destination " << outputFile << ". Permissions?" << std::endl;
        return;
    }
    //Print information for ppm file
    fprintf(f, "P6\n%d %d\n%d\n", width(), height(), 255);
    //Write the pixel data to the file
    fwrite(pixels, 1, height()*width()*3, f);
    //Close the file
    fclose(f);
}

/**
 * up()
 * Return up vector
 */
Vector3D ViewDetails::up() {
    return upVector;
}

/**
 * p
 * Return point on line created between eye and lookat point
 */
Vector3D ViewDetails::p(double t) {
    return eye() + (d() * t);
}

/**
 * d
 * Return point line created between eye and lookat point
 */
Vector3D ViewDetails::d() {
    return (s() - eye()).unit();
}

/**
 * s
 * Return lookat point
 */
Vector3D ViewDetails::s() {
    return lookAt;
}

/**
 * eye
 * Return camera position
 */
Vector3D ViewDetails::eye() {
    return position;
}

/**
 * background
 * Return background vector color
 */
Vector3D ViewDetails::background() {
    return backgroundColor;
}

/**
 * foreground
 * Return fill color vector color
 */
Vector3D ViewDetails::foreground() {
    return fillColor;
}

/**
 * height
 * Return Y resolution
 */
unsigned int ViewDetails::height() {
    return resY;
}

/**
 * Return X resolution
 */
unsigned int ViewDetails::width() {
    return resX;
}

/**
 * FoV
 * Return field of view
 */
double ViewDetails::FoV() {
    return fov;
}

/**
 * Fill
 * Set fill color
 */
void ViewDetails::Fill(Vector3D color) {
    fillColor = color;
}

/**
 * Background
 * Set background color
 */
void ViewDetails::Background(double R, double G, double B) {
    backgroundColor = Vector3D(R,G,B);
}

/**
 * Resolution
 * Set X,Y Resolution
 */
void ViewDetails::Resolution(unsigned int x, unsigned int y) {
    resX = x; resY = y;
}

/**
 * Hither
 * Set hither value
 */
void ViewDetails::Hither(double value) {
    hither = value;
}

/**
 * Angle
 * Set fov or camera angle
 */
void ViewDetails::Angle(double value) {
    fov = value;
}

/**
 * UpVec
 * Set the upwards vector
 */
void ViewDetails::UpVec(Vector3D coord) {
    upVector = coord;
}

/**
 * LookVec
 * Set the lookat vector
 */
void ViewDetails::LookVec(Vector3D coord) {
    lookAt = coord;
}

/**
 * Position
 * Set the camera position
 */
void ViewDetails::Position(Vector3D coord) {
    position = coord;
}

/**
 * w
 * return normalized w-vector LOOK
 */
Vector3D ViewDetails::w() { return d().unit(); }

/**
 * u
 * return normalized u-vector RIGHT
 */
Vector3D ViewDetails::u() { return Vector3D::cross(w(), up()); }

/**
 * v
 * return normalized v-vector UP
 */
Vector3D ViewDetails::v() { return Vector3D::cross(u(), w()); }

/**
 * d
 * Return distance to viewing pane/
 */
double ViewDetails::dist() { return d().magnitude(); }

/**
 * t
 * return t-value to viewing pane
 */
double ViewDetails::t() { return tan(FoV() * PI/360); }

/**
 * top
 * Return distance to top side of viewing pane
 */
double ViewDetails::top() { return fabs(nearPlane()) * t() * -1; }

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
void ViewDetails::AddLight(Vector3D light) {
    lights.push_back(light);
}

/**
 * CalcLighting
 * Calculate lighting of material, position, direction, normal vector given an array of vectors
 */
Vector3D ViewDetails::CalcLighting(Material material, Vector3D pos, Vector3D dir, Vector3D N, const std::vector<Actor*> *actors) const {
    Vector3D color(0,0,0);

    //Precalculate intentity of lights
    double intensity = 1.0 / sqrt(lights.size());

    for(int i = 0; i < lights.size(); i++)
    {
        //Light position
        Vector3D L = lights[i] - pos;

        //Distance actor is from light
        double dist = L.magnitude();

        //Normalized vector
        L = L.unit();

        //Find shadow actor casts from light position in direction
        Hit shadow = trace(Ray(pos,L), actors);

        //If length of shadow greater than distance from light
        if(shadow.len() > dist)
        {
            //Find to Eye vector
            Vector3D toEye = (dir * -1.0).unit();

            Vector3D H = (toEye + L).unit();

            //Dot product of normal to light
            double diffuse = MAX(0.0, Vector3D::dot(N,L));

            //Find specular value of material given Phong shine power
            double specular = pow(MAX(0.0, Vector3D::dot(N,H)), material.shader.Shine);

            //True specular value given specularity Ks
            double spec = material.shader.Ks * specular;

            //Current color * diffuse + specular
            Vector3D curr = (material.color * material.shader.Kd) + Vector3D(spec,spec,spec);

            //Enhance color with calculated light
            color = color + (curr * diffuse * intensity);
        }
    }

    return color;
}

/**
 * trace
 * Ray-Trace given a set of actors
 */
const Hit ViewDetails::trace(Ray r, const std::vector<Actor*> *actors) const {

    //Closest hit = null(essentially)
    Hit closest;

    //Loop through all actors
    for(std::vector<Actor*>::const_iterator it = actors->begin(); it != actors->end(); it++)
    {
        //Find intersection (if one exists)
        Hit current = (*it)->intersect(r);

        //Determine if it's closer than our closest actor
        if(current < closest)
        {
            //Set cloest to current actor
            closest = current;
            //Set the far distance to current distance (can't be any further due to it hitting the actor)
            r.far(current.len());
        }
    }

    return closest;
}