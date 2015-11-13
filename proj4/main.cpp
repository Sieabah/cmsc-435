#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <stdlib.h>
#include <map>
#include <string>
#include "Vector3D.h"
#include "Triangle.h"

using namespace std;

void readObject(char *fname, vector<Vector3D> &vertices, vector<Triangle> &triangles, map<string,int> &pointMap) {
    std::ifstream in(fname, std::ios::in);
    char c;
    string pt[3];
    Triangle t;

    if (!in.good()) {
        std::cerr<<"Unable to open file \""<<fname<<"\""<<std::endl;
        abort();
    }

    int idx = 0;

    while (in.good()) {
        in >> c;
        if (!in.good()) break;
        if (c == 'v') {
            in >> pt[0] >> pt[1] >> pt[2];

            pointMap[pt[0]+pt[1]+pt[2]] = idx;

            vertices.push_back(Vector3D(atof(pt[0].c_str()),
                                        atof(pt[1].c_str()),
                                        atof(pt[2].c_str())));

        } else if (c == 'f') {
            in >> t[0] >> t[1] >> t[2];
            t[0]-=1; t[1]-=1; t[2]-=1;
            triangles.push_back(t);
        }

        idx++;
    }
}

vector<Triangle> subdivide(vector<Vector3D> &verts, map<string,int> pointMap, int ione, int itwo, int ithree)
{
    Vector3D one = verts[ione];
    Vector3D two = verts[itwo];
    Vector3D three = verts[ithree];

    Vector3D ot = Vector3D::midpoint(one,two);
    Vector3D tt = Vector3D::midpoint(two,three);
    Vector3D to = Vector3D::midpoint(three,one);

    int idx[3];

    if(pointMap.find(ot.stringify()) == pointMap.end()) {
        pointMap[ot.stringify()] = idx[0] = verts.size();
        verts.push_back(ot);
    } else {
        idx[0] = pointMap[ot.stringify()];
    }

    if(pointMap.find(tt.stringify()) == pointMap.end()) {
        pointMap[tt.stringify()] = idx[1] = verts.size();
        verts.push_back(tt);
    } else {
        idx[1] = pointMap[tt.stringify()];
    }

    if(pointMap.find(to.stringify()) == pointMap.end()) {
        pointMap[to.stringify()] = idx[2] = verts.size();
        verts.push_back(to);
    } else {
        idx[2] = pointMap[to.stringify()];
    }

    vector<Triangle> end;

    //First triangle
    end.push_back(Triangle(ione, idx[0], idx[2]));

    //Second triangle
    end.push_back(Triangle(itwo, idx[2], idx[1]));

    //Third triangle
    end.push_back(Triangle(ithree, idx[0], idx[3]));

    //Fourth triangle
    end.push_back(Triangle(idx[0], idx[1], idx[2]));

    return end;
}

int main(int argc, char* argv[]) {
    vector<Vector3D> vertices;
    vector<Triangle> triangles;
    int subdivides = atoi(argv[3]);

    map<string,int> pointMap;

    readObject(argv[1], vertices, triangles, pointMap);

    vector<Triangle> newTris;

    if(pointMap.find("BUT") == pointMap.end())
        cout << "NOT FOUND";
    else
        cout << pointMap["BUT"];

    //**
    for (vector<Triangle>::iterator it = triangles.begin(); it != triangles.end(); it++) {
        vector<Triangle> newSet;
        for (int i = 0; i < subdivides; ++i) {
            subdivide(newSet, pointMap, vertices[(*it)[0]], vertices[(*it)[1]], vertices[(*it)[2]]);
        }
    }
     //*/

    return 0;
}