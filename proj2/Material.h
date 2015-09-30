/**
 * Material.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * Material instance with shader and diffuse color
 */

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H


#include "Vector3D.h"

/**
 * Shader struct
 * Holds basic shader information
 */
struct Shading{
    /**
     * Shading
     * Default constructor
     */
    Shading(){}

    /**
     * Shading
     * Initialize with parameters
     */
    Shading(double _Kd, double _Ks, double _shine, double _t, double _index)
    : Kd(_Kd), Ks(_Ks), Shine(_shine), T(_t), index_of_refraction(_index)
    {}
    //Diffuse component
    double Kd;

    //Specular
    double Ks;

    //Shine (Phong cosine)
    double Shine;

    //Transmittance
    double T;

    double index_of_refraction;
};

class Material {
public:
    /**
     * Material
     * default constructor initalizes with empty shader and no color
     */
    Material() : shader(Shading()) {}

    /**
     * Material
     * initialize material with given color and shader
     */
    Material(Vector3D colour, Shading shade): color(colour), shader(shade) {};

    /**
     * Material
     * Create material from color and shader parameters
     */
    Material(Vector3D colour, double Kd, double Ks, double shine, double t, double index):
            color(colour), shader(Shading(Kd, Ks, shine, t, index)) {};

    //Material color
    Vector3D color;

    //Material shader
    Shading shader;
};


#endif //RAYTRACER_MATERIAL_H
