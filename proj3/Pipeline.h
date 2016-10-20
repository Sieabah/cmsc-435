/**
 * Pipeline.h
 *
 * Christopher S Sidell
 * CMSC 435 Assignment 3 - Graphics Pipeline
 *
 * Implementation of the software graphics pipeline
 */

#ifndef ROOTPROJ_PIPELINE_H
#define ROOTPROJ_PIPELINE_H

#include <Eigen/Dense>
#include "Material.h"
#include "ViewDetails.h"
#include "Vertex.h"

class World;
class ViewDetails;
class Actor;

/**
 * Graphical fragment
 */
struct Fragment{
    Fragment(){
        zbuffer = 0;
    }

    Eigen::Vector3d color;
    Material material;

    double alpha;
    double beta;
    double gamma;

    std::vector<vertex> verts;
    double zbuffer;
};

/**
 * Lighting modes
 */
enum LIGHTING{ NONE, FLAT, LAMBERT, PHONG };

typedef std::vector<std::vector<Fragment> > raster;
typedef std::vector<raster> fragmentRaster;

/**
 * Graphics Pipeline (FcCG Chapter 8)
 */
class Pipeline {
private:
    /**
     * World reference
     */
    World* m_world;

    /**
     * Viewport matrix
     */
    Eigen::Matrix4d M_vp;

    /**
     * Orthogonal matrix
     */
    Eigen::Matrix4d M_ortho;

    /**
     * Perspective matrix
     */
    Eigen::Matrix4d M_persp;

    /**
     * Camera matrix
     */
    Eigen::Matrix4d M_camera;

    /**
     * M-Matrix (combination of vp*ortho*P*camera)
     */
    Eigen::Matrix4d M_matrix;

    /**
     * Enable debug output
     */
    const bool DEBUG_OUTPUT;

    /**
     * Output grayscale zBuffer
     */
    const bool OUTPUT_Z_BUFFER;

    /**
     * Culling backfacing triangle enabled
     */
    const bool CULL_ENABLED;

    /**
     * Lighting model, NONE|FLAT|LAMBERT|PHONG
     */
    LIGHTING LIGHTING_TYPE;

    /**
     * Is zbuffer set?
     */
    bool zBuffer_set;

    /**
     * zBuffer minimum
     */
    double zBuffer_min;

    /**
     * zBuffer maximum
     */
    double zBuffer_max;

protected:

    /**
     * Generate viewing matrix pg.144 FoCG 7.2
     */
    void GenerateViewMatrix();

    /**
     * Generate orthogonal matrix pg.145 FoCG 7.3
     */
    void GenerateOrthogonalMatrix();

    /**
     * Generate Camera Matrix pg.147 FoCG 7.4
     */
    void GenerateCameraMatrix();

    /**
     * Generate Perspective Matrix pg.152 & pg.154 FoCG
     */
    void GeneratePerspectiveMatrix();

    /**
     * Generate M Matrix pg.155 FoCG
     */
    void GenerateMMatrix();

public:

    /**
     * Constructor
     * @param world World reference
     * @param lighting Lighting model
     * @param debug Debug output enabled
     * @param zBuffer Output zbuffer
     * @param culling Culling enabled
     * @return
     */
    Pipeline(World *world, LIGHTING lighting = FLAT, bool debug=false,
             bool zBuffer=false, bool culling=false);

    /**
     * Run pipeline
     * @param outputFile
     */
    void run(std::string outputFile);

    /**
     * Define zBuffer values
     * @param min Min value
     * @param max Max value
     */
    void defineZBuffer(double min, double max);

protected:

    /**
     * Is this triangle backfacing?
     * @param verts Vertices
     * @return
     */
    bool bCull(std::vector<vertex> verts);

    /**
     * Vertex Processing Stage
     * @return
     */
    std::vector<Actor*> VertexProcessing();

    /**
     * Clipping stage
     * NOT IMPLEMENTED
     */
    void Clipping(){ return; }

private:
    /**
     * Barycentric coordinate function relevant for alpha (pg.169 FcOG)
     * @param x Position
     * @param y Position
     * @param xValues All vertices
     * @param yValues All vertices
     * @return
     */
    double raster_f12(double x, double y, Eigen::Vector3d &xValues, Eigen::Vector3d &yValues);

    /**
     * Barycentric coordinate function relevant for beta (pg.169 FcOG)
     * @param x Position
     * @param y Position
     * @param xValues All vertices
     * @param yValues All vertices
     * @return
     */
    double raster_f20(double x, double y, Eigen::Vector3d &xValues, Eigen::Vector3d &yValues);

    /**
     * Barycentric coordinate function relevant for gamma (pg.169 FcOG)
     * @param x Position
     * @param y Position
     * @param xValues All vertices
     * @param yValues All vertices
     * @return
     */
    double raster_f01(double x, double y, Eigen::Vector3d &xValues, Eigen::Vector3d &yValues);

protected:

    /**
     * Rasterization Stage
     * @param actors Actors to raster
     * @return
     */
    fragmentRaster Rasterization(std::vector<Actor*> &actors);


    /**
     * Phong lighting model pg.238 FcOG 10.8
     * @param view Camera details
     * @param lights All lights in scene
     * @param fragment Fragment to be shaded
     * @param color Output color reference
     */
    void PhongShading(ViewDetails *view, const std::vector<Light> *lights, Fragment &fragment, Eigen::Vector3d &color) const;

    /**
     * Lambertian lighting model pg.234 FcOG 10.1
     * @param view Camera details
     * @param lights All lights in scene
     * @param fragment Fragment to be shaded
     * @param color Output color reference
     */
    void LambertShading(ViewDetails *view, const std::vector<Light> *lights, Fragment &fragment, Eigen::Vector3d &color) const;

    /**
     * Flat lighting model
     * @param view Camera details
     * @param lights All lights in scene
     * @param fragment Fragment to be shaded
     * @param color Output color reference
     */
    void FlatShading(ViewDetails *view, const std::vector<Light> *lights, Fragment &fragment, Eigen::Vector3d &color) const;

    /**
     * No lighting model
     * @param view Camera details
     * @param lights All lights in scene
     * @param fragment Fragment to be shaded
     * @param color Output color reference
     */
    void NoShading(Fragment &fragment, Eigen::Vector3d &color) const;

    /**
     * Fragment Processing
     * @param fragments Vector of fragments
     * @return
     */
    fragmentRaster FragmentProcessing(fragmentRaster &fragments);

    /**
     * Blending
     * @param fragments
     * @return
     */
    raster Blending(fragmentRaster &fragments);

    /**
     * Save raster to output file in ppm format
     * @param outputFile
     * @param pixels
     */
    void Save(std::string outputFile, raster &pixels);
};

#endif //ROOTPROJ_PIPELINE_H
