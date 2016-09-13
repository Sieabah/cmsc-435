//
// Created by Christopher on 11/9/2015.
//

#ifndef MODELING_TRIANGLE_H_H
#define MODELING_TRIANGLE_H_H

class Triangle {
    int vertices[3];
public:
    Triangle(int i1, int i2, int i3){
        vertices[0] = i1; vertices[1] = i2; vertices[2] = i3;
    }
    int &operator[] (int i) {return vertices[i];};
};

#endif //MODELING_TRIANGLE_H_H
