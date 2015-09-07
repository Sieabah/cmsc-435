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

/**
 * PrintRenderInformation
 * Prints basic information about the renderer
 */
void Renderer::PrintRenderInformation()
{
    std::cout << "Background: " << background_color << std::endl;
    std::cout << "Fill: " << fill_color << std::endl;

    std::cout << "Shader: " << std::endl
            << "\tKs: " << shader.Ks << std::endl
            << "\tKd: " << shader.Kd << std::endl
            << "\tShine: " << shader.Shine << std::endl
            << "\tT: " << shader.T << std::endl
            << "\tindex_of_refraction: " << shader.index_of_refraction << std::endl;

    std::cout << "Position: " << position << std::endl;
    std::cout << "LookAt: " << lookAt << std::endl;
    std::cout << "UpVector: " << upVector << std::endl;
    std::cout << "Angle: " << angle << std::endl;
    std::cout << "Hither: " << hither << std::endl;
    std::cout << "Resolution: " << resX << "x" << resY << std::endl;

}
