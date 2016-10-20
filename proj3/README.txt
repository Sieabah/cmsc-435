Description:
This project is an implementation of the Graphics Pipeline with Vertex Processing, Rasterization, and Blending.

How to use:
-Compile: make

-To run:
pipeline.out <input nff> <output.ppm>

Notes:
You can change running conditions in main.cpp by changing the constants passed into the pipeline.

OUTPUT_ZBUFFER will output a zbuffer render of the image, white being closer and black being furthest, misses are solid white.
DEBUG will output all debug information to the console
CULLING_ENABLED will cull backfacing triangles
LIGHTING_MODEL will render the image with different lighting models, PHONG, LAMBERT, FLAT, NONE

Resources:

Eigen3: http://eigen.tuxfamily.org/
Utah Graphics Course: https://www.youtube.com/playlist?list=PLTKt9nyqX27zApNcEt5S50nBg8z6LJ1Ye

Book, heavily. Piazza.

Google for various C++ functions and errors that happened in the compiler, mostly irrelevant.