/**
 * World.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 1 - RayTracing
 *
 * World implementation
 */

#include "World.h"

#include <fstream>
#include <sstream>
#include <iostream>

/**
 * PrintWorldInformation
 * Prints basic world information and renderer information
 */
void World::PrintWorldInformation()
{
    std::cout << "World Information: " << std::endl;
    getRenderer()->PrintRenderInformation();
    std::cout << "# of Polygons: " << polygons.size() << std::endl;
}

/**
 * GenerateWorldFromNFF
 * Generates a world object from a nff file
 *
 * NFF File format: http://www.csee.umbc.edu/~adamb/435/nff.html
 */
World World::GenerateWorldFromNFF(std::string filepath)
{
    //Placeholder string
    std::string line;

    //File reader
    std::ifstream file(filepath.c_str());

    //General world
    World world = World();

    //Read from file
    if(file.is_open())
    {
        //Read line-by-line
        while (getline(file, line))
        {
            //Split the line on spaces
            std::istringstream df(line);

            //Offset substring by one word
            std::string sub;
            df >> sub;

            //General use variables for switch
            Polygon poly;
            double R,G,B;

            //Switch on the first letter
            switch(line[0])
            {
                //Handle Background
                case 'b':
                    df >> R; df >> G; df >> B;

                    //Set background color
                    world.getRenderer()->Background(R,G,B);
                    break;

                //Handle Viewport
                case 'v':
                    //For all values in the viewport
                    for (int i = 0; i < 6; ++i) {
                        //Holder variables
                        std::string tmp;
                        getline(file, tmp);

                        //initialize and offset
                        std::istringstream parameter(tmp);
                        parameter >> sub;

                        //Find which parameter we're reading
                        switch(tmp[0])
                        {
                            //From vector
                            case 'f':
                                parameter >> R; parameter >> G; parameter >> B;
                                //Update camera position
                                world.getRenderer()->Position(Vector3D(R,G,B));
                                break;

                            //Look at vector & angle
                            case 'a':
                                //aT
                                if(tmp[1] == 't')
                                {
                                    parameter >> R; parameter >> G; parameter >> B;
                                    //Update look-at vector
                                    world.getRenderer()->LookVec(Vector3D(R,G,B));
                                }
                                //aN
                                else
                                {
                                    parameter >> R;
                                    //Update angle
                                    world.getRenderer()->Angle(R);
                                }
                                break;

                            //Up vector
                            case 'u':
                                parameter >> R; parameter >> G; parameter >> B;

                                //Update up vector
                                world.getRenderer()->UpVec(Vector3D(R,G,B));
                                break;

                            //Hither value
                            case 'h':
                                parameter >> R;
                                //Update hither
                                world.getRenderer()->Hither(R);
                                break;

                            //Resolution
                            case 'r':
                                parameter >> R; parameter >> G;

                                //Update resolution
                                world.getRenderer()->Resolution((int)R, (int)G);
                                break;

                            //Unknown case
                            default: break;

                        }
                    }
                    break;

                //Handle Fill color and shader
                case 'f':

                    //Get the RGB values
                    df >> R; df >> G; df >> B;

                    //Define the shader
                    Shading shader;
                    df >> shader.Kd;
                    df >> shader.Ks;
                    df >> shader.Shine;
                    df >> shader.T;
                    df >> shader.index_of_refraction;

                    //Update shader and fill color
                    world.getRenderer()->Fill(Vector3D(R,G,B));
                    world.getRenderer()->Shader(shader);

                    break;

                //Handle poly creation
                case 'p':
                    //Get vertices count
                    int count;
                    df >> count;

                    //For all vertices
                    for (int j = 0; j < count; ++j) {
                        //Holder variables
                        getline(file, line);
                        std::istringstream coords(line);

                        //Create coordinate and read from line
                        Vector3D coord;
                        coords >> coord.x;
                        coords >> coord.y;
                        coords >> coord.z;

                        //Add vertex to polygon
                        poly.addVert(coord);
                    }

                    //Add polygon to world
                    world.AddPoly(poly);
                    break;

                //Unknown command case
                default: break;
            }
        }
        //Close the file
        file.close();
    }

    //Return generated world
    return world;
}