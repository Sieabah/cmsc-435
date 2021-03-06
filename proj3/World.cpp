/**
 * World.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * World header definition
 */

#include <fstream>
#include <sstream>
#include <iostream>

#include "World.h"
#include "Polygon.h"
#include "Actor.h"

/**
 * World()
 * Default constructor
 */
World::World() : camera(ViewDetails()) {}

/**
 * Destructor
 */
World::~World() {
    //For all actors
    while(!actors.empty()){
        //Delete
        delete actors.back();
        actors.pop_back();
    }
}

/**
 * PrintWorldInformation
 * Prints basic world information and renderer information
 */
void World::PrintWorldInformation()
{
    std::cout << "World Information: " << std::endl;
    std::cout << "# of Actors: " << actors.size() << std::endl;
    getRenderer()->PrintRenderInformation();
}

/**
 * GenerateWorldFromNFF
 * Generates a world object from a nff file
 *
 * NFF File format: http://www.csee.umbc.edu/~adamb/435/nff.html
 */
bool World::GenerateWorldFromNFF(std::string filepath, World &world)
{
    //Placeholder string
    std::string line;

    //File reader
    std::ifstream file(filepath.c_str());

    //Read from file
    if(!file.is_open()) {
        return false;
    }

    //General variables
    Material material;
    ViewDetails *view = world.getRenderer();

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
                view->Background(R,G,B);
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
                            view->Position(Eigen::Vector3d(R,G,B));
                            break;

                        //Look at vector & fov
                        case 'a':
                            //aT
                            if(tmp[1] == 't')
                            {
                                parameter >> R; parameter >> G; parameter >> B;
                                //Update look-at vector
                                view->LookVec(Eigen::Vector3d(R,G,B));
                            }
                            //aN
                            else
                            {
                                parameter >> R;
                                //Update fov
                                view->Angle(R);
                            }
                            break;

                        //Up vector
                        case 'u':
                            parameter >> R; parameter >> G; parameter >> B;

                            //Update up vector
                            view->UpVec(Eigen::Vector3d(R,G,B));
                            break;

                        //Hither value
                        case 'h':
                            parameter >> R;
                            //Update hither
                            view->Hither(R);
                            break;

                        //Resolution
                        case 'r':
                            parameter >> R; parameter >> G;

                            //Update resolution
                            view->Resolution((unsigned int)R, (unsigned int)G);
                            break;

                        //Unknown case
                        default: break;

                    }
                }
                break;

            //Light
            case 'l':
                df >> R; df >> G; df >> B;
                world.AddLight(Eigen::Vector3d(R,G,B));
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
                view->Fill(Eigen::Vector3d(R, G, B));

                //Update the material we're assigning to Actors
                material = Material(Eigen::Vector3d(R, G, B), shader);
            }
                break;

            //Handle poly creation
            case 'p': {
                //Get vertices count
                int count;
                df >> count;

                std::vector<Polygon*> polys;

                //Polygon patch
                if(line[1] == 'p'){
                    Polygon *poly = new Polygon();

                    //For all vertices
                    for (int j = 0; j < 3; ++j) {
                        //Holder variables
                        getline(file, line);
                        std::istringstream coords(line);

                        double X,Y,Z, n1, n2, n3;

                        //Create coordinate and read from line
                        coords >> X;
                        coords >> Y;
                        coords >> Z;

                        //Create coordinate and read from line
                        coords >> n1;
                        coords >> n2;
                        coords >> n3;

                        //Add vertex to polygon
                        poly->addVert(Eigen::Vector3d(X, Y, Z), Eigen::Vector3d(n1, n2, n3));

                    }
                    polys.push_back(poly);
                } else {
                    //Generate only triangle polygons
                    polys = generatePolys(file, line, count);
                }


                //Finalize polygons
                for(std::vector<Polygon*>::iterator it = polys.begin(); it < polys.end(); it++){
                    //Assign current material to poly actor
                    (*it)->addMaterial(material);

                    //Precalculate values for verticies (RAY TRACING ONLY)
                    //(*it)->finalize();

                    //Add polygon to world
                    world.AddActor((*it));
                }
            }
                break;

            //Unknown command case
            default: break;
        }
    }

    //Close the file
    file.close();

    //Return generated world
    return true;
}

/**
 * Polys
 * Return point to polygon vector
 */
const std::vector<Actor*> * World::Actors() const {
    return &actors;
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
ViewDetails * World::getRenderer() {
    return &camera;
}

/**
 * AddLight
 * Adds light to world
 */
void World::AddLight(const Eigen::Vector3d &light) {
    getRenderer()->AddLight(light);
}

/**
 * Generate triangles
 * @param file File pointer
 * @param line Current line
 * @param count vertex count
 * @return
 */
std::vector<Polygon*> World::generatePolys(std::ifstream &file, std::string &line, int &count) {
    std::vector<Polygon*> polys;

    //If it is three, continue as normal
    if(count == 3) {
        Polygon *poly = new Polygon();

        //For all vertices
        for (int j = 0; j < 3; ++j) {
            //Holder variables
            getline(file, line);
            std::istringstream coords(line);

            double X,Y,Z;

            //Create coordinate and read from line
            coords >> X;
            coords >> Y;
            coords >> Z;

            //Add vertex to polygon
            poly->addVert(Eigen::Vector3d(X, Y, Z));
        }
        polys.push_back(poly);

    //If count is 4, build them 0-1-2 2-3-0
    } else if(count == 4){
        std::vector<Eigen::Vector3d> vectors;

        //For all vertices
        for (int j = 0; j < 4; ++j) {
            //Holder variables
            getline(file, line);
            std::istringstream coords(line);

            double X,Y,Z;

            //Create coordinate and read from line
            coords >> X;
            coords >> Y;
            coords >> Z;

            //Add vertex to polygon
            vectors.push_back(Eigen::Vector3d(X, Y, Z));
        }

        Polygon *poly1 = new Polygon();
        Polygon *poly2 = new Polygon();

        //First triangle 0-1-2
        poly1->addVert(vectors[0]);
        poly1->addVert(vectors[1]);
        poly1->addVert(vectors[2]);

        //Second triangle 2-3-0
        poly2->addVert(vectors[2]);
        poly2->addVert(vectors[3]);
        poly2->addVert(vectors[0]);

        polys.push_back(poly1);
        polys.push_back(poly2);

    //I don't want to do more math to figure out the actual shape
    } else {
        std::cout << "ARBITRARY POLYGONS NOT SUPPORTED" << std::endl;
    }

    return polys;
}
