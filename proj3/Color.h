//
// Created by Christopher on 10/16/2016.
//

#ifndef ROOTPROJ_COLOR_H
#define ROOTPROJ_COLOR_H

#include <Eigen/Dense>
/**
 * Color
 * Struct to convert vector color to a 24-bit color
 */
struct Color{
    //Convert color to 8-bit color
    static unsigned char Convert(double value)
    {
        if(value < 0) return (unsigned char) 0;
        //Handle possible overflow (green/yellow super colors)
        return (unsigned char) (value > 1 ? 255 : int(255*value + 0.5));
    };

    Color(double r, double g, double b){
        R = Convert(r);
        G = Convert(g);
        B = Convert(b);
    }

    Color(Eigen::Vector3d vec){
        R = Convert(vec(0));
        G = Convert(vec(1));
        B = Convert(vec(2));
    }

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

#endif //ROOTPROJ_COLOR_H
