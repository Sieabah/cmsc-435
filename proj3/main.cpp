/**
 * main.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * This project demonstrates basic ray tracing
 */

#include <iostream>
#include "World.h"
#include "Pipeline.h"
#include <sys/stat.h>

using namespace std;

/**
 * Checks for file existence
 */
inline bool file_exits(const string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

int main(int argc, char *argv[]) {
    //Check for amount of arguments
    if(argc < 3)
    {
        cout << "usage: <program> input.nff output.ppm" << endl;
        return 1;
    }
    //Check if file actually exists
    else if(!file_exits(argv[1]))
    {
        cout << "Please give a valid input file" << endl;
        return 1;
    }

    //Create world from NFF file
    World world = World::GenerateWorldFromNFF(string(argv[1]));

    cout << "Pipeline" << endl;
    Pipeline rendering = Pipeline(&world);

    cout << "VertexProcessing" << endl;
    rendering.VertexProcessing();

    cout << "Rasterization" << endl;
    rendering.Rasterization();

    cout << "Blending" << endl;
    rendering.Blending(argv[2]);

    cout << "End" << endl;
    //Render the world
    //world.Render(argv[2]);

    return 0;
}
