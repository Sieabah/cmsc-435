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

#include <string>
#include <vector>

#include <sys/stat.h>
#include <stdio.h>


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
        cout << "Please specify the file to display: <program> <nff file> <output file>" << endl;
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

    //world.Render(argv[2],world);


    return 0;
}
