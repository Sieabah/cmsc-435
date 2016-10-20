/**
 * Light.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * Constitutes a light which has position and intensity
 */

#ifndef ROOTPROJ_LIGHT_H
#define ROOTPROJ_LIGHT_H

#include <Eigen/Dense>

#include "Material.h"

/**
 * Light Component
 */
class Light {
    /**
     * 3D position of light in world coordinates
     */
    Eigen::Vector3d m_position;

    /**
     * Intensity of light
     */
    double m_intensity;

public:
    /**
     * Get light position
     * @return Light position
     */
    const Eigen::Vector3d position() const{
        return m_position;
    }

    /**
     * Get light intensity
     * @return Light intensity
     */
    const double intensity() const{
        return m_intensity;
    }

    /**
     * General constructor for light
     * @param x World coordinate
     * @param y World coordinate
     * @param z World coordinate
     * @param light_intensity Light intensity between [0, max_double]
     * @return Light component
     */
    Light(double x, double y, double z, double light_intensity = 1){
        m_position = Eigen::Vector3d(x,y,z);
        m_intensity = std::max(0.0, light_intensity);
    }

    /**
     * General constructor for light
     * @param pos Position in world coordinates
     * @param light_intensity Light intensity between [0, max_double]
     * @return Light component
     */
    Light(Eigen::Vector3d pos = Eigen::Vector3d(0,0,0), double light_intensity = 1):
            m_position(pos), m_intensity(std::max(0.0, light_intensity)){
    }

    /**
     * Flat shading color
     * @param pointPos Point we're trying to shade
     * @param look Look (eye) position
     * @param normal Face normal
     * @param material Material of component
     * @param lightPos Light position
     * @return Resulting color
     */
    Eigen::Vector3d GetColor(Eigen::Vector3d pointPos, Eigen::Vector3d look, Eigen::Vector3d normal, Material material, Eigen::Vector3d lightPos) const{
        Eigen::Vector3d color(0,0,0);

        double channel = material.shader.Kd * intensity();

        Eigen::Vector3d I = lightPos-pointPos;
        Eigen::Vector3d v = look-pointPos;

        double bias = (I).normalized().dot(normal.normalized());

        color(0) = material.color(0) * channel*bias;
        color(1) = material.color(1) * channel*bias;
        color(2) = material.color(2) * channel*bias;

        return color;
    }
};


#endif //ROOTPROJ_LIGHT_H
