/**
 * Renderer.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * The implementation of the renderer
 */

#include "Renderer.h"
#include <iostream>
#include <stdio.h>

/**
 * Color
 * Struct to convert vector color to a 24-bit color
 */
struct Color{
    //Convert color to 8-bit color
    unsigned char Convert(double value)
    {
        return (unsigned char)(value*255);
    };
    //Take vector and convert it to 3 color channels
    Color(Vector3D vec){
        R = Convert(vec.x);
        G = Convert(vec.y);
        B = Convert(vec.z);
    };
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

/**
 * Renderer
 * Default constructor
 */
Renderer::Renderer(): position(Vector3D()), lookAt(Vector3D()), upVector(Vector3D()),
                      fov(0), hither(0), resX(640),resY(480),
                      backgroundColor(Vector3D(0,0,0)), fillColor(Vector3D(255,0,0)){};

/**
 * PrintRenderInformation
 * Prints basic information about the renderer
 */
void Renderer::PrintRenderInformation()
{
    std::cout << "Background: " << backgroundColor << std::endl;
    std::cout << "Fill: " << fillColor << std::endl;

    std::cout << "Shader: " << std::endl
            << "\tKs: " << shader.Ks << std::endl
            << "\tKd: " << shader.Kd << std::endl
            << "\tShine: " << shader.Shine << std::endl
            << "\tT: " << shader.T << std::endl
            << "\tindex_of_refraction: " << shader.index_of_refraction << std::endl;

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

}

/**
 * Render
 * outputFile - The filename to be written to
 * polys - A pointer to the vector of polygons in the scene
 * printout - Print the output to the console or output file
 */
void Renderer::Render(std::string outputFile, std::vector<Polygon>* polys,  bool printout) {
    //Determine the W V U vectors
    Vector3D W = (eye() - s()).unit();
    Vector3D V = up().unit();
    Vector3D U = Vector3D::cross(V, W).unit();

    //Focal length
    double focal = d().magnitude();

    //Length from focal point to top/bottom edge
    double focalHeight = focal * tanf((float)(FoV() * PI / 360.0));

    //Length from focal point to right/left edge
    double focalWidth = width() * focalHeight / height();

    //Create pixel array
    unsigned char pixels[height()][width()][3];

    //For every row
    for (int i = 0; i < height(); ++i) {
        //For every column
        for (int j = 0; j < width(); ++j) {
            //Determine u & v vectors (from book)
            double u = (2.0 * focalWidth * (j + 0.5) / width())-focalWidth;
            double v = (2.0 * focalWidth * (i + 0.5) / height())-focalHeight;

            //Precalculate ray direction
            Vector3D dir = (W * -focal) + (V * -v) + (U * u);

            //For expansion later, convert background/foreground vector color to 24-bit colors
            Color bgColor = Color(background());
            Color fillColor = Color(foreground());

            //Set background on pixel
            pixels[i][j][0] = bgColor.R;
            pixels[i][j][1] = bgColor.G;
            pixels[i][j][2] = bgColor.B;

            //For all polygons in the world
            for(std::vector<Polygon>::iterator it = polys->begin(); it != polys->end(); it++)
            {
                //Determine if ray intersects with poly
                if(it->intersects(eye(),dir))
                {
                    //Set color to fill color
                    pixels[i][j][0] = fillColor.R;
                    pixels[i][j][1] = fillColor.G;
                    pixels[i][j][2] = fillColor.B;

                    //Break if we intersect
                    break;
                }
            }
        }
    }

    //Given by prompt
    //Open file
    FILE *f = fopen(outputFile.c_str(),"wb");
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
Vector3D Renderer::up() {
    return upVector;
}

/**
 * p
 * Return point on line created between eye and lookat point
 */
Vector3D Renderer::p(double t) {
    return eye() + (d() * t);
}

/**
 * d
 * Return point line created between eye and lookat point
 */
Vector3D Renderer::d() {
    return s() - eye();
}

/**
 * s
 * Return lookat point
 */
Vector3D Renderer::s() {
    return lookAt;
}

/**
 * eye
 * Return camera position
 */
Vector3D Renderer::eye() {
    return position;
}

/**
 * background
 * Return background vector color
 */
Vector3D Renderer::background() {
    return backgroundColor;
}

/**
 * foreground
 * Return fill color vector color
 */
Vector3D Renderer::foreground() {
    return fillColor;
}

/**
 * height
 * Return Y resolution
 */
unsigned int Renderer::height() {
    return resY;
}

/**
 * Return X resolution
 */
unsigned int Renderer::width() {
    return resX;
}

/**
 * FoV
 * Return field of view
 */
double Renderer::FoV() {
    return fov;
}

/**
 * Shader
 * Set shader
 */
void Renderer::Shader(Shading newShader) {
    shader = newShader;
}

/**
 * Fill
 * Set fill color
 */
void Renderer::Fill(Vector3D color) {
    fillColor = color;
}

/**
 * Background
 * Set background color
 */
void Renderer::Background(double R, double G, double B) {
    backgroundColor = Vector3D(R,G,B);
}

/**
 * Resolution
 * Set X,Y Resolution
 */
void Renderer::Resolution(unsigned int x, unsigned int y) {
    resX = x; resY = y;
}

/**
 * Hither
 * Set hither value
 */
void Renderer::Hither(double value) {
    hither = value;
}

/**
 * Angle
 * Set fov or camera angle
 */
void Renderer::Angle(double value) {
    fov = value;
}

/**
 * UpVec
 * Set the upwards vector
 */
void Renderer::UpVec(Vector3D coord) {
    upVector = coord;
}

/**
 * LookVec
 * Set the lookat vector
 */
void Renderer::LookVec(Vector3D coord) {
    lookAt = coord;
}

/**
 * Position
 * Set the camera position
 */
void Renderer::Position(Vector3D coord) {
    position = coord;
}
