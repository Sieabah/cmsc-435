/**
 * Pipeline.cpp
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * Implementation of the software graphics pipeline
 */

#include "Pipeline.h"

#include <utility>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include "World.h"
#include "Actor.h"
#include "Polygon.h"
#include "Color.h"

/**
 * Generate viewing matrix pg.144 FoCG 7.2
 */
void Pipeline::GenerateViewMatrix() {
    ViewDetails *view = m_world->getRenderer();

    M_vp = Eigen::Matrix4d::Identity();

    M_vp(0,0) = ((double)view->width())/2.0;
    M_vp(1,1) = ((double)view->height())/2.0;
    M_vp(0,3) = ((double)view->width() - 1.0)/2.0;
    M_vp(1,3) = ((double)view->height() - 1.0)/2.0;

    if(DEBUG_OUTPUT)
        std::cout << "ViewMatrix" << std::endl << M_vp << std::endl;
}

/**
 * Generate orthogonal matrix pg.145 FoCG 7.3
 */
void Pipeline::GenerateOrthogonalMatrix() {
    ViewDetails *view = m_world->getRenderer();

    M_ortho = Eigen::Matrix4d::Identity();

    //X
    M_ortho(0, 0) = 2.0 / (view->right() - view->left());
    M_ortho(0, 3) = -(view->right() + view->left()) / (view->right() - view->left());

    //Y
    M_ortho(1, 1) = 2.0 / (view->top() - view->bottom());
    M_ortho(1, 3) = -(view->top() + view->bottom()) / (view->top() - view->bottom());

    //Z
    M_ortho(2, 2) = 2/(view->nearPlane() - view->farPlane());
    M_ortho(2, 3) = -(view->nearPlane() + view->farPlane()) / (view->nearPlane() - view->farPlane());

    if(DEBUG_OUTPUT)
        std::cout << "Orthogonal" << std::endl << M_ortho << std::endl;
}

/**
 * Generate Camera Matrix pg.147 FoCG 7.4
 */
void Pipeline::GenerateCameraMatrix() {
    ViewDetails *view = m_world->getRenderer();

    //LOOK
    Eigen::Vector3d w = view->w();

    //RIGHT
    Eigen::Vector3d u = view->u();

    //UP
    Eigen::Vector3d v = view->v();

    M_camera(0,0) = u(0);
    M_camera(0,1) = u(1);
    M_camera(0,2) = u(2);

    M_camera(1,0) = v(0);
    M_camera(1,1) = v(1);
    M_camera(1,2) = v(2);

    M_camera(2,0) = w(0);
    M_camera(2,1) = w(1);
    M_camera(2,2) = w(2);

    M_camera(3,3) = 1.0;

    if(DEBUG_OUTPUT)
        std::cout << "Pre-Eye Camera" << std::endl << M_camera << std::endl;

    Eigen::Matrix4d eyeMatrix = Eigen::Matrix4d::Identity();

    eyeMatrix(0,3) = -view->eye()(0);
    eyeMatrix(1,3) = -view->eye()(1);
    eyeMatrix(2,3) = -view->eye()(2);

    if(DEBUG_OUTPUT)
        std::cout << "Eye" << std::endl << eyeMatrix << std::endl;

    M_camera = M_camera * eyeMatrix;

    if(DEBUG_OUTPUT)
        std::cout << "Camera" << std::endl << M_camera << std::endl;
}

/**
 * Generate Perspective Matrix pg.152 & pg.154 FoCG
 */
void Pipeline::GeneratePerspectiveMatrix() {
    ViewDetails *view = m_world->getRenderer();

    M_persp(0,0) = view->nearPlane();

    M_persp(1,1) = view->nearPlane();

    M_persp(2,2) = view->nearPlane()+view->farPlane();

    M_persp(2,3) = -view->farPlane()*view->nearPlane();

    M_persp(3,2) = 1.0;


    if(DEBUG_OUTPUT)
        std::cout << "P-Matrix" << std::endl << M_persp << std::endl;

    M_persp = M_ortho * M_persp;

    if(DEBUG_OUTPUT)
        std::cout << "Perspective" << std::endl << M_persp << std::endl;
}

/**
 * Generate M Matrix pg.155 FoCG
 */
void Pipeline::GenerateMMatrix() {
    M_vp = Eigen::Matrix4d::Zero();
    M_ortho = Eigen::Matrix4d::Zero();
    M_persp = Eigen::Matrix4d::Zero();
    M_camera = Eigen::Matrix4d::Zero();
    M_matrix = Eigen::Matrix4d::Zero();

    GenerateViewMatrix();
    GenerateOrthogonalMatrix();
    GeneratePerspectiveMatrix();
    GenerateCameraMatrix();

    M_matrix = M_vp * M_persp * M_camera;

    if(DEBUG_OUTPUT)
        std::cout << "M-Matrix" << std::endl << M_matrix << std::endl;
}

/**
 * Constructor
 * @param world World reference
 * @param lighting Lighting model
 * @param debug Debug output enabled
 * @param zBuffer Output zbuffer
 * @param culling Culling enabled
 * @return
 */
Pipeline::Pipeline(World *world, double alpha, LIGHTING lighting, bool debug,
                   bool zBuffer, bool culling)
        : m_world(world), ALPHA(alpha), DEBUG_OUTPUT(debug), OUTPUT_Z_BUFFER(zBuffer),
          CULL_ENABLED(culling), LIGHTING_TYPE(lighting){

    if(DEBUG_OUTPUT) {
        std::cout << "Pipeline" << std::endl;
        std::cout << "Lighting Model " << LIGHTING_TYPE << std::endl;
        world->PrintWorldInformation();
    }

    zBuffer_min = 0;
    zBuffer_max = 0;
    zBuffer_set = false;
}

/**
 * Run pipeline
 * @param outputFile
 */
void Pipeline::run(std::string outputFile) {
    GenerateMMatrix();

    //Run vertex processing
    std::cout << "Step 1/5 VP" << std::endl;
    std::vector<Actor*> actors = VertexProcessing();

    //Run Clipping
    //std::cout << "Step 2/5 C" << std::endl;
    //std::vector<Actor*> actors = Clipping();

    //Run rasterization
    std::cout << "Step 3/5 R" << std::endl;
    fragmentRaster fragments = Rasterization(actors);

    //Run fragment processing
    std::cout << "Step 4/5 FP" << std::endl;
    fragments = FragmentProcessing(fragments);

    //Run blending
    std::cout << "Step 5/5 B" << std::endl;
    raster Raster = Blending(fragments);

    //Save output
    Save(outputFile, Raster);
}

/**
 * Define zBuffer values
 * @param min Min value
 * @param max Max value
 */
void Pipeline::defineZBuffer(double min, double max) {
    zBuffer_max = max;
    zBuffer_min = min;
    zBuffer_set = true;
}

bool Pipeline::bCull(std::vector<vertex> verts) {
    Eigen::Vector4d original_p1(verts[0].pos(0), verts[0].pos(1), verts[0].pos(2), 1.0);
    Eigen::Vector4d original_p2(verts[1].pos(0), verts[1].pos(1), verts[1].pos(2), 1.0);
    Eigen::Vector4d original_p3(verts[2].pos(0), verts[2].pos(1), verts[2].pos(2), 1.0);

    original_p1 = M_camera * original_p1;
    original_p2 = M_camera * original_p2;
    original_p3 = M_camera * original_p3;

    Eigen::Vector3d p1(original_p1(0), original_p1(1), original_p1(2));
    Eigen::Vector3d p2(original_p2(0), original_p2(1), original_p2(2));
    Eigen::Vector3d p3(original_p3(0), original_p3(1), original_p3(2));

    Eigen::Vector3d V = p2 - p1;
    Eigen::Vector3d W = p3 - p1;

    Eigen::Vector3d normal = V.cross(W);

    return normal.dot(p1) > 0;

}

/**
 * Vertex Processing Stage
 * @return
 */
std::vector<Actor *> Pipeline::VertexProcessing() {
    if(DEBUG_OUTPUT)
        std::cout << "VertexProcessing" << std::endl;

    //Get a reference to all actors in world
    const std::vector<Actor*>* actors = m_world->Actors();

    //Create array for actors to be processed
    std::vector<Actor*> actorsToProcess;

    //Loop over all actors
    for(std::vector<Actor*>::const_iterator it = actors->begin(); it < actors->end(); it++){
        //(For simplicity rename the pointer)
        Actor *actor = (*it);

        //Get vertices
        std::vector<vertex> verts = *actor->getVerticies();

        //Cull if it's enabled
        if(CULL_ENABLED && bCull(verts))
            continue;

        //Loop over all
        for(std::vector<vertex>::iterator jt = actor->getVerticies()->begin(); jt < actor->getVerticies()->end(); jt++){
            //Create homogenous vector
            Eigen::Vector4d vec(jt->pos(0), jt->pos(1), jt->pos(2), 1.0);

            //Tranform vector by M_Matrix
            vec = M_matrix * vec;

            //Have to do this to work with windows
            jt->camera_pos = new Eigen::Vector4d(vec);
        }

        //Push actor to list to pass on
        actorsToProcess.push_back(actor);
    }

    //Output debug information if relevant
    if(DEBUG_OUTPUT && CULL_ENABLED)
        std::cout << actorsToProcess.size() << "/" << actors->size()
                  << ((double)actors->size()-actorsToProcess.size())/actors->size()*100
                  << "% culled" << std::endl;

    return actorsToProcess;
}

/**
 * Barycentric coordinate function relevant for alpha (pg.169 FcOG)
 * @param x Position
 * @param y Position
 * @param xValues All vertices
 * @param yValues All vertices
 * @return
 */
double Pipeline::raster_f12(double x, double y,
                            Eigen::Vector3d &xValues, Eigen::Vector3d &yValues){
    return (yValues[1] - yValues[2])*x
           + (xValues[2] - xValues[1])*y
           + xValues[1]*yValues[2] - xValues[2]*yValues[1];
}

/**
 * Barycentric coordinate function relevant for beta (pg.169 FcOG)
 * @param x Position
 * @param y Position
 * @param xValues All vertices
 * @param yValues All vertices
 * @return
 */
double Pipeline::raster_f20(double x, double y,
                            Eigen::Vector3d &xValues, Eigen::Vector3d &yValues){
    return (yValues[2] - yValues[0])*x
           + (xValues[0] - xValues[2])*y
           + xValues[2]*yValues[0] - xValues[0]*yValues[2];
}

/**
 * Barycentric coordinate function relevant for gamma (pg.169 FcOG)
 * @param x Position
 * @param y Position
 * @param xValues All vertices
 * @param yValues All vertices
 * @return
 */
double Pipeline::raster_f01(double x, double y,
                            Eigen::Vector3d &xValues, Eigen::Vector3d &yValues){
    return (yValues[0] - yValues[1])*x
           + (xValues[1] - xValues[0])*y
           + xValues[0]*yValues[1] - xValues[1]*yValues[0];
}

/**
 * Rasterization Stage
 * @param actors Actors to raster
 * @return
 */
fragmentRaster Pipeline::Rasterization(std::vector<Actor *> &actors) {
    if(DEBUG_OUTPUT)
        std::cout << "Rasterization" << std::endl;

    //Get camera details
    ViewDetails *view = m_world->getRenderer();

    //Debug variable
    double minmax[] = {0, 0, 0, 0};

    //Create fragments collection
    fragmentRaster fragments;

    //Properly size it
    fragments.resize(view->height());
    for(fragmentRaster::iterator i = fragments.begin(); i < fragments.end(); i++){
        i->resize(view->width());
    }

    //For all actors
    for(std::vector<Actor*>::const_iterator it = actors.begin(); it < actors.end(); it++) {
        //Simplify pointer name
        Actor *actor = (*it);

        //Get vertices of triangle
        std::vector<vertex> verts = *(actor->getVerticies());

        //Homogenous Divide x coordinates
        Eigen::Vector3d xValues(
                verts[0].camera_pos->coeffRef(0)/verts[0].camera_pos->coeffRef(3),
                verts[1].camera_pos->coeffRef(0)/verts[1].camera_pos->coeffRef(3),
                verts[2].camera_pos->coeffRef(0)/verts[2].camera_pos->coeffRef(3)
        );

        //Homogenous Divide y coordinates
        Eigen::Vector3d yValues(
                verts[0].camera_pos->coeffRef(1)/verts[0].camera_pos->coeffRef(3),
                verts[1].camera_pos->coeffRef(1)/verts[1].camera_pos->coeffRef(3),
                verts[2].camera_pos->coeffRef(1)/verts[2].camera_pos->coeffRef(3)
        );

        //Homogenous Divide z coordinates
        Eigen::Vector3d zValues(
                verts[0].camera_pos->coeffRef(2)/verts[0].camera_pos->coeffRef(3),
                verts[1].camera_pos->coeffRef(2)/verts[1].camera_pos->coeffRef(3),
                verts[2].camera_pos->coeffRef(2)/verts[2].camera_pos->coeffRef(3)
        );

        //Find minX and maxX
        int minX = (int)floor(std::max((double)0.0, xValues.minCoeff()));
        int maxX = (int)ceil(std::min((double)view->width(), xValues.maxCoeff()));

        //Debug values
        minmax[0] = xValues.minCoeff() < minmax[0] ? xValues.minCoeff() : minmax[0];
        minmax[2] = xValues.maxCoeff() > minmax[2] ? xValues.maxCoeff() : minmax[2];

        //Find minY and maxY
        int minY = (int)floor(std::max((double)0.0, yValues.minCoeff()));
        int maxY = (int)ceil(std::min((double)view->height(), yValues.maxCoeff()));

        //Debug values
        minmax[1] = yValues.minCoeff() < minmax[1] ? yValues.minCoeff() : minmax[1];
        minmax[3] = yValues.maxCoeff() > minmax[3] ? yValues.maxCoeff() : minmax[3];

        //For entire raster
        for(int y = minY; y < maxY; y++){
            for(int x = minX; x < maxX; x++){

                //Calculate barycentric coordinates for triangle
                double alpha = raster_f12(x,y, xValues, yValues)/raster_f12(xValues(0), yValues(0), xValues, yValues);
                double beta = raster_f20(x,y, xValues, yValues)/raster_f20(xValues(1), yValues(1), xValues, yValues);
                double gamma = raster_f01(x,y, xValues, yValues)/raster_f01(xValues(2), yValues(2), xValues, yValues);

                //Is point in triangle?
                if(alpha > 0 && beta > 0 && gamma > 0) {
                    Eigen::Vector3d &A = verts[0].pos;
                    Eigen::Vector3d &B = verts[1].pos;
                    Eigen::Vector3d &C = verts[2].pos;

                    //Create fragment
                    Fragment fragment = Fragment();
                    fragment.zbuffer = zValues[0] * alpha + zValues[1] * beta + zValues[2] * gamma;
                    fragment.alpha = alpha;
                    fragment.beta = beta;
                    fragment.gamma = gamma;
                    fragment.verts = verts;
                    fragment.color = actor->Texture().color;
                    fragment.material = actor->Texture();
                    fragment.normal = (B-A).cross(C-A).normalized();

                    //Save fragment on that point
                    fragments[y][x].push_back(fragment);
                }
            }
        }
    }

    if(DEBUG_OUTPUT) {
        std::cout << "min: " << minmax[0] << ", " << minmax[1] << std::endl;
        std::cout << "max: " << minmax[2] << ", " << minmax[3] << std::endl;
    }

    return fragments;
}

/**
 * Phong lighting model pg.238 FcOG 10.8
 * @param view Camera details
 * @param lights All lights in scene
 * @param fragment Fragment to be shaded
 * @param color Output color reference
 */
void Pipeline::PhongShading(ViewDetails *view, const std::vector<Light> *lights,
                            Fragment &fragment, Eigen::Vector3d &color) const {
    Eigen::Vector3d resultColor(0, 0, 0);

    //If no lights, there is no color
    if (lights->size() == 0) {
        color = resultColor;
        return;
    }

    //Get specific point in triangle
    Eigen::Vector3d &normal = fragment.normal;

    if(fragment.verts[0].bNormals && fragment.verts[1].bNormals && fragment.verts[2].bNormals){
        normal = fragment.verts[0].normal*fragment.alpha
                + fragment.verts[1].normal*fragment.beta
                + fragment.verts[2].normal*fragment.gamma;
    }

    //Get specific point in triangle
    Eigen::Vector3d point = fragment.verts[0].pos*fragment.alpha
                            + fragment.verts[1].pos*fragment.beta
                            + fragment.verts[2].pos*fragment.gamma;

    //Get diffuse color
    Eigen::Vector3d diffuse = fragment.material.color * fragment.material.shader.Kd;

    //For all lights in scene
    for(std::vector<Light>::const_iterator light = lights->begin(); light < lights->end(); light++){
        //Get light intensity
        double intensity = light->intensity()/sqrt(lights->size());

        //Calculate direction to light form point
        Eigen::Vector3d lightDir = (light->position() - point).normalized();

        //Calculate vector halfway between lightDir and Eye
        Eigen::Vector3d h = (view->eye()+lightDir).normalized();

        double h_normal = h.dot(normal);

        double phongHighlight = 0;

        //Calculate phongHighlight
        if(h_normal > 0)
            phongHighlight = intensity*pow(h_normal, fragment.material.shader.Shine);

        //Calculate specular amount
        double specular = fragment.material.shader.Ks * phongHighlight;

        //Diffuse color
        Eigen::Vector3d tmpColor = diffuse * (intensity * std::max(0.0, normal.dot(lightDir)));

        //Add highlights
        tmpColor += Eigen::Vector3d(specular, specular, specular);

        //Add color to diffuse
        resultColor += tmpColor;
    }

    color = resultColor;
}

/**
 * Lambertian lighting model pg.234 FcOG 10.1
 * @param view Camera details
 * @param lights All lights in scene
 * @param fragment Fragment to be shaded
 * @param color Output color reference
 */
void Pipeline::LambertShading(ViewDetails *view, const std::vector<Light> *lights,
                              Fragment &fragment, Eigen::Vector3d &color) const {
    Eigen::Vector3d resultColor(0, 0, 0);

    //If no lights, there is no color
    if (lights->size() == 0) {
        color = resultColor;
        return;
    }

    //Get specific point in triangle
    Eigen::Vector3d &normal = fragment.normal;

    if(fragment.verts[0].bNormals && fragment.verts[1].bNormals && fragment.verts[2].bNormals){
        normal = fragment.verts[0].normal*fragment.alpha
                     + fragment.verts[1].normal*fragment.beta
                     + fragment.verts[2].normal*fragment.gamma;
    }

    //Calculate specific point on triangle
    Eigen::Vector3d point = fragment.verts[0].pos*fragment.alpha
                            + fragment.verts[1].pos*fragment.beta
                            + fragment.verts[2].pos*fragment.gamma;


    //Get diffuse color
    Eigen::Vector3d diffuse = fragment.material.color * fragment.material.shader.Kd;

    for(std::vector<Light>::const_iterator light = lights->begin(); light < lights->end(); light++){
        //Light intensity
        double Cl = light->intensity()/sqrt(lights->size());

        //Get light direction
        Eigen::Vector3d lightDir = (light->position() - point).normalized();

        //Add resulting color
        resultColor += diffuse * normal.dot(lightDir);
    }

    color = resultColor;
}

/**
 * Flat lighting model
 * @param view Camera details
 * @param lights All lights in scene
 * @param fragment Fragment to be shaded
 * @param color Output color reference
 */
void Pipeline::FlatShading(ViewDetails *view, const std::vector<Light> *lights, Fragment &fragment,
                           Eigen::Vector3d &color) const {
    color = Eigen::Vector3d(0, 0, 0);

    //If no lights, there is no color
    if (lights->size() == 0) {
        return;
    }

    //Get face normal
    Eigen::Vector3d &normal = fragment.normal;

    //Get position on face
    Eigen::Vector3d facePosition(fragment.verts[0].pos);

    //For all lights
    for (std::vector<Light>::const_iterator light = lights->begin(); light < lights->end(); light++) {

        //Get resulting color from light
        Eigen::Vector3d result = light->GetColor(facePosition, view->eye(),
                                                 normal, fragment.material,
                                                 light->position());
        color(0) += std::max(result(0), 0.0);
        color(1) += std::max(result(1), 0.0);
        color(2) += std::max(result(2), 0.0);
    }
}

/**
 * No lighting model
 * @param view Camera details
 * @param lights All lights in scene
 * @param fragment Fragment to be shaded
 * @param color Output color reference
 */
void Pipeline::NoShading(Fragment &fragment, Eigen::Vector3d &color) const {
    color = fragment.color;
}

/**
 * Fragment Processing
 * @param fragments Vector of fragments
 * @return
 */
fragmentRaster Pipeline::FragmentProcessing(fragmentRaster &fragments) {
    if(DEBUG_OUTPUT)
        std::cout << "Fragment Processing" << std::endl;

    //Get camera details
    ViewDetails *view = m_world->getRenderer();

    //Get reference of lights
    const std::vector<Light> *lights = view->getLights();

    //For all pixels
    for(int y = 0; y < view->height(); y++) {
        for (int x = 0; x < view->width(); x++) {
            //For all fragments at that pixel
            for(std::vector<Fragment>::iterator fragment = fragments[y][x].begin(); fragment < fragments[y][x].end(); fragment++){
                //Calculate light
                Eigen::Vector3d color;

                //Allow different types of lighting
                if(LIGHTING_TYPE == PHONG)
                    PhongShading(view, lights, *fragment, color);
                else if(LIGHTING_TYPE == LAMBERT)
                    LambertShading(view, lights, *fragment, color);
                else if(LIGHTING_TYPE == FLAT)
                    FlatShading(view, lights, *fragment, color);
                else
                    NoShading(*fragment, color);

                fragment->color = color;
            }
        }
    }

    return fragments;
}

/**
 * Blending
 * @param fragments
 * @return
 */
raster Pipeline::Blending(fragmentRaster &fragments) {
    if(DEBUG_OUTPUT)
        std::cout << "Blending" << std::endl;
    ViewDetails *view = m_world->getRenderer();

    //Create end raster
    raster Raster;

    //Size to correct size
    Raster.resize(view->height());
    for(raster::iterator i = Raster.begin(); i < Raster.end(); i++){
        i->resize(view->width());
    }

    //For all pixels
    for(int y = 0; y < view->height(); y++){
        for(int x = 0; x < view->width(); x++) {
            //Set default zbuffer to far plane
            Raster[y][x].zbuffer = view->farPlane();
            Raster[y][x].color = view->background();

            //If there are no fragments background color is used
            if(fragments[y][x].size() == 0){
                continue;
            } else {
                std::sort(fragments[y][x].begin(), fragments[y][x].end());

                if(ALPHA < 1){
                    double alpha = ALPHA, oldPixels = (1.0 - ALPHA);

                    for (std::vector<Fragment>::iterator fragment = fragments[y][x].begin();
                         fragment < fragments[y][x].end(); fragment++) {
                        Raster[y][x].color = Eigen::Vector3d(fragment->color * alpha)
                                             + Eigen::Vector3d(Raster[y][x].color * oldPixels);
                    }
                } else {
                    Raster[y][x] = fragments[y][x].back();
                }
            }
        }
    }

    return Raster;
}

/**
 * Save raster to output file in ppm format
 * @param outputFile
 * @param pixels
 */
void Pipeline::Save(std::string outputFile, raster &pixels) {
    if(DEBUG_OUTPUT)
        std::cout << "Saving to file " << outputFile << std::endl;

    ViewDetails *view = m_world->getRenderer();

    //Create buffer
    unsigned char output[view->width()][view->height()][3];

    //If outputting zbuffer and it's not manually set
    if(OUTPUT_Z_BUFFER && !zBuffer_set){
        zBuffer_min = view->nearPlane();
        for(int y = 0; y < view->height(); y++){
            for(int x = 0; x < view->width(); x++){
                //Calculate range of min to max

                if(pixels[y][x].zbuffer == view->farPlane()) continue;

                pixels[y][x].zbuffer = fabs(pixels[y][x].zbuffer);

                zBuffer_min = pixels[y][x].zbuffer < zBuffer_min ? pixels[y][x].zbuffer : zBuffer_min;
                zBuffer_max = pixels[y][x].zbuffer > zBuffer_max ? pixels[y][x].zbuffer : zBuffer_max;
            }
        }
    }

    //yOffset for flipping pixels correctly for PPM format
    int yOffset = view->height()-1;

    //For all pixels
    for(int y = 0; y < view->height(); y++){
        for(int x = 0; x < view->width(); x++){

            //Get fragment
            Fragment &offsetPixel = pixels[yOffset - y][x];

            if(OUTPUT_Z_BUFFER){
                //Output grayscale
                double color = (fabs(offsetPixel.zbuffer)-zBuffer_min)/zBuffer_max;

                output[y][x][0] = Color::Convert(color);
                output[y][x][1] = Color::Convert(color);
                output[y][x][2] = Color::Convert(color);
            } else {
                //Get color of pixel
                Color color(offsetPixel.color);

                output[y][x][0] = color.R;
                output[y][x][1] = color.G;
                output[y][x][2] = color.B;
            }
        }
    }

    //Given by prompt
    //Open file
    FILE *f = fopen(outputFile.c_str(),"wb");
    //Print information for ppm file
    fprintf(f, "P6\n%d %d\n%d\n", view->width(), view->height(), 255);
    //Write the pixel data to the file
    fwrite(output, 1, view->height()*view->width()*3, f);
    //Close the file
    fclose(f);
}