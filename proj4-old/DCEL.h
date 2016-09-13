//
// Created by Christopher on 11/9/2015.
//

#ifndef MODELING_DCEL_H
#define MODELING_DCEL_H

#include "Vector3D.h"

struct DCELHalfEdge;
struct DCELVert;
struct DCELFace;

struct DCELFace
{
    DCELHalfEdge *edge;
};

struct DCELHalfEdge
{
    DCELVert *origin;
    DCELFace *face;

    DCELHalfEdge *twin;
    DCELHalfEdge *next;
};

struct DCELVert
{
    DCELHalfEdge* leaving;
};

class DCEL {
public:
    void AddFace(Vector3D one, Vector3D two, Vector3D three)
    {

    }
};


#endif //MODELING_DCEL_H
