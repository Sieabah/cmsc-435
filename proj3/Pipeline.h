//
// Created by Christopher on 10/14/2016.
//

#ifndef ROOTPROJ_PIPELINE_H
#define ROOTPROJ_PIPELINE_H

#include <Eigen/Dense>
#include <utility>
#include <algorithm>
#include <stdio.h>
#include "World.h"
#include "ViewDetails.h"
#include "Actor.h"
#include "Polygon.h"
#include "Color.h"

struct Fragment{
    Fragment(){}

    Eigen::Vector3d xValues;
    Eigen::Vector3d yValues;
    Eigen::Vector3d zValues;

    Eigen::Vector3d color;
    Material material;
    std::pair<int, int> pixel;

    double alpha;
    double beta;
    double gamma;

    std::vector<vertex> verts;
    double zbuffer = 0;
};

enum LIGHTING{ FLAT, PHONG };

typedef std::vector<std::vector<Fragment>> raster;
typedef std::vector<std::vector<Fragment>> fragmentCollection;
typedef std::vector<fragmentCollection> fragmentRaster;

class Pipeline {
private:
    World* m_world;
    Eigen::Matrix4d M_vp;
    Eigen::Matrix4d M_ortho;
    Eigen::Matrix4d M_persp;
    Eigen::Matrix4d M_camera;
    Eigen::Matrix4d M_matrix;

    const bool DEBUG_OUTPUT;
    const bool OUTPUT_Z_BUFFER;
    const bool CULL_ENABLED;
    LIGHTING LIGHTING_TYPE;

protected:
    void GenerateViewMatrix();
    void GenerateOrthogonalMatrix();
    void GenerateCameraMatrix();
    void GeneratePerspectiveMatrix();
    void GenerateMMatrix();

public:

    Pipeline(World *world, LIGHTING lighting = FLAT, bool debug=false, bool zBuffer=false, bool culling=false): m_world(world),
    DEBUG_OUTPUT(debug), OUTPUT_Z_BUFFER(zBuffer), CULL_ENABLED(culling), LIGHTING_TYPE(lighting){
        if(DEBUG_OUTPUT)
            std::cout << "Pipeline" << std::endl;

        ViewDetails *view = m_world->getRenderer();
    }

    void run(std::string outputFile) {
        GenerateMMatrix();

        std::cout << "Step 1/5 VP" << std::endl;
        std::vector<Actor*> actors = VertexProcessing();

        std::cout << "Step 2/5 C" << std::endl;
        //std::vector<Actor*> actors = Clipping();

        std::cout << "Step 3/5 R" << std::endl;
        fragmentRaster fragments = Rasterization(actors);

        std::cout << "Step 4/5 FP" << std::endl;
        fragments = FragmentProcessing(fragments);

        std::cout << "Step 5/5 B" << std::endl;
        raster Raster = Blending(fragments);

        Save(outputFile, Raster);
    }
protected:

    bool bCull(std::vector<vertex> verts){
        Eigen::Vector4d original_p1(verts[0].pos.x, verts[0].pos.y, verts[0].pos.z, 1.0);
        Eigen::Vector4d original_p2(verts[1].pos.x, verts[1].pos.y, verts[1].pos.z, 1.0);
        Eigen::Vector4d original_p3(verts[2].pos.x, verts[2].pos.y, verts[2].pos.z, 1.0);

        original_p1 = M_camera * original_p1;
        original_p2 = M_camera * original_p2;
        original_p3 = M_camera * original_p3;

        Eigen::Vector3d p1(original_p1(0), original_p1(1), original_p1(2));
        Eigen::Vector3d p2(original_p2(0), original_p2(1), original_p2(2));
        Eigen::Vector3d p3(original_p3(0), original_p3(1), original_p3(2));

        Eigen::Vector3d V = p2 - p1;
        Eigen::Vector3d W = p3 - p1;

        Eigen::Vector3d eV(V(0), V(1), V(2));
        Eigen::Vector3d eW(W(0), W(1), W(2));

        Eigen::Vector3d normal = eV.cross(eW);

        return normal.dot(p1) > 0;

    }
    std::vector<Actor*> VertexProcessing(){
        if(DEBUG_OUTPUT)
            std::cout << "VertexProcessing" << std::endl;

        const std::vector<Actor*>* actors = m_world->Actors();

        std::vector<Actor*> actorsToProcess;

        for(std::vector<Actor*>::const_iterator it = actors->begin(); it < actors->end(); it++){
            Actor *actor = (*it);

            std::vector<vertex> verts = *actor->getVerticies();

            if(CULL_ENABLED && bCull(verts))
                continue;

            for(std::vector<vertex>::iterator jt = actor->getVerticies()->begin(); jt < actor->getVerticies()->end(); jt++){
                Eigen::Vector4d vec(jt->pos.x, jt->pos.y, jt->pos.z, 1.0);
                vec = M_matrix * vec;

                //Have to do this to work with windows
                jt->camera_pos = new Eigen::Vector4d(vec);
            }

            actorsToProcess.push_back(actor);
        }

        if(DEBUG_OUTPUT && CULL_ENABLED)
            std::cout << actorsToProcess.size() << "/" << actors->size()
                      << ((double)actors->size()-actorsToProcess.size())/actors->size()*100 << "% culled" << std::endl;

        return actorsToProcess;
    }

    void Clipping(){
        return;
    }

private:
    double raster_f12(double x, double y, Eigen::Vector3d &xValues, Eigen::Vector3d &yValues){
        return (yValues[1] - yValues[2])*x + (xValues[2] - xValues[1])*y + xValues[1]*yValues[2] - xValues[2]*yValues[1];
    }
    double raster_f20(double x, double y, Eigen::Vector3d &xValues, Eigen::Vector3d &yValues){
        return (yValues[2] - yValues[0])*x + (xValues[0] - xValues[2])*y + xValues[2]*yValues[0] - xValues[0]*yValues[2];
    }

    double raster_f01(double x, double y, Eigen::Vector3d &xValues, Eigen::Vector3d &yValues) {
        return (yValues[0] - yValues[1])*x + (xValues[1] - xValues[0])*y + xValues[0]*yValues[1] - xValues[1]*yValues[0];
    }

protected:

    fragmentRaster Rasterization(std::vector<Actor*> &actors){
        if(DEBUG_OUTPUT)
            std::cout << "Rasterization" << std::endl;

        ViewDetails *view = m_world->getRenderer();

        double minmax[] = {0, 0, 0, 0};

        fragmentRaster fragments;

        fragments.resize(view->height());
        for(fragmentRaster::iterator i = fragments.begin(); i < fragments.end(); i++){
            i->resize(view->width());
        }

        for(std::vector<Actor*>::const_iterator it = actors.begin(); it < actors.end(); it++) {
            Actor *actor = (*it);

            std::vector<vertex> verts = *(actor->getVerticies());

            Eigen::Vector3d xValues(
                    verts[0].camera_pos->coeffRef(0)/verts[0].camera_pos->coeffRef(3),
                    verts[1].camera_pos->coeffRef(0)/verts[1].camera_pos->coeffRef(3),
                    verts[2].camera_pos->coeffRef(0)/verts[2].camera_pos->coeffRef(3)
            );

            Eigen::Vector3d yValues(
                    verts[0].camera_pos->coeffRef(1)/verts[0].camera_pos->coeffRef(3),
                    verts[1].camera_pos->coeffRef(1)/verts[1].camera_pos->coeffRef(3),
                    verts[2].camera_pos->coeffRef(1)/verts[2].camera_pos->coeffRef(3)
            );

            Eigen::Vector3d zValues(
                    verts[0].camera_pos->coeffRef(2)/verts[0].camera_pos->coeffRef(3),
                    verts[1].camera_pos->coeffRef(2)/verts[1].camera_pos->coeffRef(3),
                    verts[2].camera_pos->coeffRef(2)/verts[2].camera_pos->coeffRef(3)
            );

            int minX = (int)floor(std::max((double)0.0, xValues.minCoeff()));
            int maxX = (int)ceil(std::min((double)view->width(), xValues.maxCoeff()));

            minmax[0] = xValues.minCoeff() < minmax[0] ? xValues.minCoeff() : minmax[0];
            minmax[2] = xValues.maxCoeff() > minmax[2] ? xValues.maxCoeff() : minmax[2];

            int minY = (int)floor(std::max((double)0.0, yValues.minCoeff()));
            int maxY = (int)ceil(std::min((double)view->height(), yValues.maxCoeff()));

            minmax[1] = yValues.minCoeff() < minmax[1] ? yValues.minCoeff() : minmax[1];
            minmax[3] = yValues.maxCoeff() > minmax[3] ? yValues.maxCoeff() : minmax[3];

            for(int y = minY; y < maxY; y++){
                for(int x = minX; x < maxX; x++){
                    double alpha = raster_f12(x,y, xValues, yValues)/raster_f12(xValues(0), yValues(0), xValues, yValues);
                    double beta = raster_f20(x,y, xValues, yValues)/raster_f20(xValues(1), yValues(1), xValues, yValues);
                    double gamma = raster_f01(x,y, xValues, yValues)/raster_f01(xValues(2), yValues(2), xValues, yValues);

                    if(alpha > 0 && beta > 0 && gamma > 0) {
                        Fragment fragment = Fragment();
                        fragment.zbuffer = zValues[0] * alpha + zValues[1] * beta + zValues[2] * gamma;
                        fragment.xValues = xValues;
                        fragment.yValues = yValues;
                        fragment.zValues = zValues;
                        fragment.alpha = alpha;
                        fragment.beta = beta;
                        fragment.gamma = gamma;
                        fragment.verts = verts;
                        fragment.pixel.first = x;
                        fragment.pixel.second = y;
                        fragment.color = Eigen::Vector3d(actor->Texture().color.x, actor->Texture().color.y, actor->Texture().color.z);
                        fragment.material = actor->Texture();

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

    void PhongShading(ViewDetails *view, const std::vector<Light> *lights, Fragment &fragment, Eigen::Vector3d &color) const {
        color = Eigen::Vector3d(view->foreground().x, view->foreground().y, view->foreground().z);

    }

    void FlatShading(ViewDetails *view, const std::vector<Light> *lights, Fragment &fragment, Eigen::Vector3d &color) const {
        //@todo per-pixel coloring
        Vector3D V = fragment.verts[1].pos - fragment.verts[0].pos;
        Vector3D W = fragment.verts[2].pos - fragment.verts[0].pos;

        Eigen::Vector3d eV(V.x, V.y, V.z);
        Eigen::Vector3d eW(W.x, W.y, W.z);

        Eigen::Vector3d normal = eV.cross(eW);

        if (lights->size() == 0) {
            return;
        }
        Eigen::Vector3d resultColor(0, 0, 0);

        Eigen::Vector3d facePosition(fragment.verts[0].pos.x, fragment.verts[0].pos.y, fragment.verts[0].pos.z);

        for (std::vector<Light>::const_iterator light = lights->begin(); light < lights->end(); light++) {
            Eigen::Vector3d result = light->GetColor(facePosition, view->eye(), normal, fragment.material,
                                                     light->position());

            resultColor(0) += std::max(result(0), 0.0);
            resultColor(1) += std::max(result(1), 0.0);
            resultColor(2) += std::max(result(2), 0.0);
        }

        color = resultColor;
    }

    fragmentRaster FragmentProcessing(fragmentRaster &fragments){
        if(DEBUG_OUTPUT)
            std::cout << "Fragment Processing" << std::endl;

        ViewDetails *view = m_world->getRenderer();

        const std::vector<Light> *lights = view->getLights();

        for(int y = 0; y < view->height(); y++) {
            for (int x = 0; x < view->width(); x++) {
                for(std::vector<Fragment>::iterator fragment = fragments[y][x].begin(); fragment < fragments[y][x].end(); fragment++){
                    Eigen::Vector3d color;

                    switch(LIGHTING_TYPE) {
                        PHONG:
                            PhongShading(view, lights, *fragment, color);
                            break;
                        default:
                            FlatShading(view, lights, *fragment, color);
                    }

                    fragment->color = color;
                }
            }
        }

        return fragments;
    }

    raster Blending(fragmentRaster &fragments){
        if(DEBUG_OUTPUT)
            std::cout << "Blending" << std::endl;
        ViewDetails *view = m_world->getRenderer();

        raster Raster;

        Raster.resize(view->height());
        for(raster::iterator i = Raster.begin(); i < Raster.end(); i++){
            i->resize(view->width());
        }

        for(int y = 0; y < view->height(); y++){
            for(int x = 0; x < view->width(); x++) {
                Raster[y][x].zbuffer = view->farPlane();
                if(fragments[y][x].size() == 0){
                    Raster[y][x].color = Eigen::Vector3d(view->background().x, view->background().y, view->background().z);
                } else {
                    for(auto fragment = fragments[y][x].begin(); fragment < fragments[y][x].end(); fragment++){
                        if(fragment->zbuffer < Raster[y][x].zbuffer)
                            Raster[y][x] = *fragment;
                    }
                }
            }
        }

        return Raster;
    }

private:
    bool zBuffer_set = false;
    double zBuffer_min = 0;
    double zBuffer_max = 0;
public:
    void defineZBuffer(double min, double max){
        zBuffer_max = max;
        zBuffer_min = min;
        zBuffer_set = true;
    }

protected:

    void Save(std::string outputFile, raster &pixels){
        if(DEBUG_OUTPUT)
            std::cout << "Saving to file " << outputFile << std::endl;

        ViewDetails *view = m_world->getRenderer();

        unsigned char output[view->width()][view->height()][3];

        if(OUTPUT_Z_BUFFER && !zBuffer_set){
            zBuffer_min = view->nearPlane();
            for(int y = 0; y < view->height(); y++){
                for(int x = 0; x < view->width(); x++){
                    pixels[y][x].zbuffer = fabs(pixels[y][x].zbuffer);
                    if(pixels[y][x].zbuffer == view->farPlane()) continue;

                    zBuffer_min = pixels[y][x].zbuffer < zBuffer_min ? pixels[y][x].zbuffer : zBuffer_min;
                    zBuffer_max = pixels[y][x].zbuffer > zBuffer_max ? pixels[y][x].zbuffer : zBuffer_max;
                }
            }
        }

        int yOffset = view->height()-1;
        for(int y = 0; y < view->height(); y++){
            for(int x = 0; x < view->width(); x++){
                Fragment &offsetPixel = pixels[yOffset - y][x];
                Fragment &savePixel = pixels[y][x];
                if(OUTPUT_Z_BUFFER){
                    double color = (fabs(offsetPixel.zbuffer)-zBuffer_min)/zBuffer_max;

                    output[y][x][0] = Color::Convert(color);
                    output[y][x][1] = Color::Convert(color);
                    output[y][x][2] = Color::Convert(color);
                } else {
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
};


#endif //ROOTPROJ_PIPELINE_H
