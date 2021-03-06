/**
 * World.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 2 - RayTracing II
 *
 * World header definition
 */

#include "World.h"
#include "Polygon.h"


#include <fstream>
#include <sstream>
#include <iostream>

/**
 * World()
 * Default constructor
 */
World::World() : camera(Renderer()) {}

/**
 * PrintWorldInformation
 * Prints basic world information and renderer information
 */
void World::PrintWorldInformation()
{
    std::cout << "World Information: " << std::endl;
    getRenderer()->PrintRenderInformation();
    std::cout << "# of Actors: " << actors.size() << std::endl;
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
        Material material;

        //Read line-by-line
        while (getline(file, line))
        {
            //Split the line on spaces
            std::istringstream df(line);

            //Offset substring by one word
            std::string sub;
            df >> sub;

            //General use variables for switch
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

                            //Look at vector & fov
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
                                    //Update fov
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
                                world.getRenderer()->Resolution((unsigned int)R, (unsigned int)G);
                                break;

                            //Unknown case
                            default: break;

                        }
                    }
                    break;

                //Light
                case 'l':
                    df >> R; df >> G; df >> B;
                    world.AddLight(Vector3D(R,G,B));
                    break;

                //Handle Fill color and shader
                case 'f': {
                    //Get the RGB values
                    df >> R;
                    df >> G;
                    df >> B;

                    //Define the shader
                    Shading shader;
                    df >> shader.Kd;
                    df >> shader.Ks;
                    df >> shader.Shine;
                    df >> shader.T;
                    df >> shader.index_of_refraction;

                    //Update shader and fill color
                    world.getRenderer()->Fill(Vector3D(R, G, B));

                    //Update the material we're assigning to Actors
                    material = Material(Vector3D(R, G, B), shader);
                }
                    break;

                //Handle poly creation
                case 'p': {
                    //Get vertices count
                    int count;
                    df >> count;
                    Polygon *poly = new Polygon();

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
                        poly->addVert(coord);
                    }

                    //Assign current material to poly actor
                    poly->addMaterial(material);

                    //Precalculate values for verticies
                    poly->finalize();

                    //Add polygon to world
                    world.AddActor(poly);
                }
                    break;

                //Unknown command case
                default: break;
            }
        }
        //Close the file
        file.close();
    }

    //world.PrintWorldInformation();
    //Return generated world
    return world;
}

/**
 * Polys
 * Return point to polygon vector
 */
const std::vector<Actor*> * World::Actors() const {
    return &actors;
}

/**
 * Render
 * Pass the variable to the camera render
 */
void World::Render(std::string outputFile, bool printout) {
    //Sorry didn't have time to implement this
    printout = false;
    camera.Render(outputFile, Actors(), *this, printout);
}

/**
 * AddPoly
 * Add polygon to vector
 */
void World::AddActor(Actor *actor)
{
    actors.push_back(actor);
}

/**
 * getRenderer
 * Returns pointer to renderer
 */
Renderer * World::getRenderer() {
    return &camera;
}

/**
 * AddLight
 * Adds light to world
 */
void World::AddLight(const Vector3D &light) {
    getRenderer()->AddLight(light);
}