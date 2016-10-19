//
// Created by Christopher on 10/18/2016.
//

#ifndef ROOTPROJ_LIGHT_H
#define ROOTPROJ_LIGHT_H

#include <Eigen/Dense>
#include "Material.h"

class Light {
    Eigen::Vector3d m_position;
    double m_intensity;

public:
    const Eigen::Vector3d position() const{
        return m_position;
    }
    const double intensity() const{
        return m_intensity;
    }

    Light(double x, double y, double z, double light_intensity = 1):Light(Eigen::Vector3d(x,y,z), light_intensity){
    }

    Light(Eigen::Vector3d pos = Eigen::Vector3d(0,0,0), double light_intensity = 1):m_position(pos), m_intensity(light_intensity){
    }

    Eigen::Vector3d GetColor(Eigen::Vector3d pointPos, Eigen::Vector3d look, Eigen::Vector3d normal, Material material, Eigen::Vector3d lightPos) const{
        Eigen::Vector3d color(0,0,0);

        double channel = material.shader.Kd * intensity();

        Eigen::Vector3d I = lightPos-pointPos;
        Eigen::Vector3d v = look-pointPos;

        double bias = (I).normalized().dot(normal.normalized());

        color(0) = material.color.x * channel*bias;
        color(1) = material.color.y * channel*bias;
        color(2) = material.color.z * channel*bias;

        return color;
    }
};


#endif //ROOTPROJ_LIGHT_H
