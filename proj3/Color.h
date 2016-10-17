//
// Created by Christopher on 10/16/2016.
//

#ifndef ROOTPROJ_COLOR_H
#define ROOTPROJ_COLOR_H

/**
 * Color
 * Struct to convert vector color to a 24-bit color
 */
struct Color{
    //Convert color to 8-bit color
    unsigned char Convert(double value)
    {
        //Handle possible overflow (green/yellow super colors)
        return (unsigned char) (value > 1 ? 255 : int(255*value + 0.5));
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

#endif //ROOTPROJ_COLOR_H
