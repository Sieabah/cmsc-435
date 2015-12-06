#define cimg_display 0
#include "CImg.h"
#include "slVector.H"
using namespace cimg_library;

struct pixelEnergy
{
    pixelEnergy(): horizontal(0), vertical(0)
    {}
    pixelEnergy(SlVector3 up, SlVector3 right, SlVector3 down, SlVector3 left)
    {
        horizontal = mag(left - right);
        vertical = mag(up - down);
    }
    double vertical;
    double horizontal;
};

int main(int argc, char *argv[]) {
    CImg<double> input(argv[1]);
    CImg<double> lab = input.RGBtoLab();
    SlVector3 *image = new SlVector3[input.width()*input.height()];
    for (unsigned int i=0; i<input.width(); i++) 
    {
        for (unsigned int j=0; j<input.height(); j++) 
        {
            image[i*input.height()+j][0] = lab(i, j, 0);
            image[i*input.height()+j][1] = lab(i, j, 1);
            image[i*input.height()+j][2] = lab(i, j, 2);
        }
    }

    pixelEnergy *energyMatrix = new pixelEnergy[input.width()*input.height()];

    for (unsigned int i=0; i<input.width(); i++) 
    {
        for (unsigned int j=0; j<input.height(); j++) 
        {
            /*image[i*input.height()+j][0] = lab(i, j, 0);
            image[i*input.height()+j][1] = lab(i, j, 1);
            image[i*input.height()+j][2] = lab(i, j, 2);*/
            SlVector3 up = image[i*(input.height()-1)+j];
            SlVector3 right = image[i*input.height()+j+1];
            SlVector3 down = image[i*(input.height()+1)+j];
            SlVector3 left = image[i*input.height()+j-1];

            energyMatrix[i*input.height()+j] = pixelEnergy(up, right, down, left);

        }
    }

    CImg<double> output(atoi(argv[3]), atoi(argv[4]), input.depth(), input.spectrum(), 0);
    for (unsigned int i=0; i<output.width(); i++) 
    {
        for (unsigned int j=0; j<output.height(); j++) 
        {
            output(i, j, 0) = image[i*output.height()+j][0];
            output(i, j, 1) = image[i*output.height()+j][1];
            output(i, j, 2) = image[i*output.height()+j][2];
        }
    }
    CImg<double> rgb = output.LabtoRGB();

    if (strstr(argv[2], "png"))
        rgb.save_png(argv[2]);
    else if (strstr(argv[2], "jpg"))
        rgb.save_jpeg(argv[2]);

    delete [] image;
    delete [] energyMatrix;
    return 0;
}
