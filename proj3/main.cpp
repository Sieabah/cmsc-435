/**
 * main.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * This project demonstrates basic ray tracing
 */

#include <iostream>
#include <sys/stat.h>

#include "World.h"
#include "Pipeline.h"

using namespace std;

const bool OUTPUT_ZBUFFER = false;
const bool DEBUG = true;
const bool CULLING_ENABLED = false;
const LIGHTING LIGHTING_MODEL = PHONG;

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
    World world = World();

    if(!World::GenerateWorldFromNFF(string(argv[1]), world)){
        std::cout << "Could not create valid world" << std::endl;
        return 1;
    };

    Pipeline pipeline(&world, LIGHTING_MODEL, DEBUG, OUTPUT_ZBUFFER, CULLING_ENABLED);
    pipeline.run(argv[2]);

    cout << "End" << endl;
    return 0;
}
