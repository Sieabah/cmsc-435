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
#include <sys/stat.h>


using namespace std;

/**
 * Checks for file existance
 */
inline bool file_exits(const string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

int main(int argc, char *argv[]) {
    //Check for amount of arguments
    if(argc < 2)
    {
        cout << "Please specify the file to display: <program> <filepath>" << endl;
        return 1;
    }
    //Check if file actually exists
    else if(!file_exits(argv[1]))
    {
        cout << "Please give a valid file" << endl;
        return 1;
    }

    //Create world from NFF file
    World world = World::GenerateWorldFromNFF(string(argv[1]));

    world.PrintWorldInformation();

    cout << endl;

    Renderer* camera = world.getRenderer();
    cout << "e: " << camera->eye() << endl;
    cout << "d: " << camera->d() << endl;

    cout << "p(0): " << camera->p(0) << endl;
    cout << "p(1): " << camera->p(1) << endl;
    return 0;
}
