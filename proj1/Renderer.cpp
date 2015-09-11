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

void Renderer::Render(std::string outputFile, std::vector<Polygon>* polys,  bool printout) {
    Vector3D W = (eye() - s()).unit();
    Vector3D V = up().unit();
    Vector3D U = Vector3D::cross(V, W).unit();

    double focal = d().magnitude();
    double halfHeight = focal * tanf((float)(FoV() * PI / 360.0f));
    double halfWidth = width() * halfHeight / height();

    unsigned char pixels[height()][width()][3];

    for (int i = 0; i < height(); ++i) {
        for (int j = 0; j < width(); ++j) {
            double u = (2.0f * halfWidth * (j + 0.5f) / width())-halfWidth;
            double v = (2.0f * halfWidth * (i + 0.5f) / height())-halfHeight;

            Vector3D dir = (W * -focal) + (V * -v) + (U * u);

            for(std::vector<Polygon>::iterator it = polys->begin(); it != polys->end(); it++)
            {
                int pixel = i*width()*j;
                if(it->intersects(eye(),dir))
                {
                    pixels[i][j][0] = (unsigned char)(foreground().x*255);
                    pixels[i][j][1] = (unsigned char)(foreground().y*255);
                    pixels[i][j][2] = (unsigned char)(foreground().z*255);
                    break;
                }
                else
                {
                    pixels[i][j][0] = (unsigned char)(background().x*255);
                    pixels[i][j][1] = (unsigned char)(background().y*255);
                    pixels[i][j][2] = (unsigned char)(background().z*255);
                }
            }


        }
    }

    FILE *f = fopen(outputFile.c_str(),"wb");
    fprintf(f, "P6\n%d %d\n%d\n", width(), height(), 255);
    fwrite(pixels, 1, height()*width()*3, f);
    fclose(f);
}
