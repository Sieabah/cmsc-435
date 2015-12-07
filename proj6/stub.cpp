#define cimg_display 0
#define debug_energy_map 0
#include "CImg.h"
#include "slVector.H"
#include "Seam.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace cimg_library;

struct pixelEnergy
{
    pixelEnergy(): horizontal(0), vertical(0)
    {}
    double vertical;
    double horizontal;
};

SlVector3* getPixel(SlVector3* img, int x, int y, int height, int width)
{
    if(x < 0 || y < 0 || x >= width || y >= height)
        return NULL;

    return &img[x+y];
}

void energyMap(std::vector<pixelEnergy> &matrix, std::vector<SlVector3> &image, int i, int j, int width, int height)
{
    if(j == 0)
        matrix[i*height+j].horizontal = mag(image[i*height+j] - image[i*height+j+1]);
    else if (j+1 >= height)
        matrix[i*height+j].horizontal = mag(image[i*height+j-1] - image[i*height+j]);
    else
        matrix[i*height+j].horizontal = mag(image[i*height+j-1] - image[i*height+j+1]);

    if(i == 0)
        matrix[i*height+j].vertical = mag(image[i*height+j] - image[(i+1)*height+j]);
    else if (i+1 >= width)
        matrix[i*height+j].vertical = mag(image[(i-1)*height+j] - image[i*height+j]);
    else
        matrix[i*height+j].vertical = mag(image[(i-1)*height+j] - image[(i+1)*height+j]);
}

int main(int argc, char *argv[]) {
    CImg<double> input(argv[1]);
    CImg<double> lab = input.RGBtoLab();
    std::vector<SlVector3> image(input.width()*input.height());
    for (unsigned int i=0; i<input.width(); i++) 
    {
        for (unsigned int j=0; j<input.height(); j++) 
        {
            image[i*input.height()+j][0] = lab(i, j, 0);
            image[i*input.height()+j][1] = lab(i, j, 1);
            image[i*input.height()+j][2] = lab(i, j, 2);
        }
    }

    std::vector<pixelEnergy> energyMatrix(input.width()*input.height());
    //std::vector<SlVector3> energyImage(input.width()*input.height());

    //Precalculate energy of all pixels in horizontal and vertical space
    for (unsigned int i=0; i<input.width(); i++) 
    {
        for (unsigned int j=0; j<input.height(); j++) 
        {

            energyMap(energyMatrix, image, i, j, input.width(), input.height());

            #if debug_energy_map
            energyImage[i*input.height()+j][0] = energyMatrix[i*input.height()+j].horizontal;
            energyImage[i*input.height()+j][1] = energyMatrix[i*input.height()+j].horizontal;
            energyImage[i*input.height()+j][2] = energyMatrix[i*input.height()+j].horizontal;
            #endif
        }
    }


    //*
    #if debug_energy_map
    CImg<double> energyOutput(input.width(), input.height(), input.depth(), input.spectrum(), 0);
    for (unsigned int i=0; i<input.width(); i++) 
    {
        for (unsigned int j=0; j<input.height(); j++) 
        {
            energyOutput(i,j,0) = energyImage[i*input.height()+j][0];
            energyOutput(i,j,1) = energyImage[i*input.height()+j][1];
            energyOutput(i,j,2) = energyImage[i*input.height()+j][2];
        }
    }

    energyOutput.save_png("energy.png");
    #endif
    //*/
    int rowsLeft = input.width() - atoi(argv[3]);
    int colsLeft = input.height() - atoi(argv[4]);

    int imageWidth = input.width();
    int imageHeight = input.height();

    while(rowsLeft || colsLeft)
    {
        if(rowsLeft)
        {  
            rowsLeft--;

            Seam seams[imageWidth];
            for (int i = 0; i < imageWidth; ++i)
            {
                int row = i;
                int col = 0;

                seams[i].add(row,col,energyMatrix[row*imageHeight+col].horizontal);
                while(col < imageHeight-1)
                {
                    double botLeft, botCenter, botRight, min;

                    if(row == 0)
                    {
                        botLeft = -1;
                        botCenter = energyMatrix[(row)*imageHeight+(col+1)].horizontal;
                        botRight = energyMatrix[(row+1)*imageHeight+(col+1)].horizontal;

                        min = std::min(botCenter, botRight);
                    }
                    else if(row == imageWidth)
                    {
                        botLeft = energyMatrix[(row-1)*imageHeight+(col+1)].horizontal;
                        botCenter = energyMatrix[(row+1)*imageHeight+(col+1)].horizontal;
                        botRight = -1;

                        min = std::min(botCenter, botLeft);
                    }
                    else
                    {
                        botLeft = energyMatrix[(row-1)*imageHeight+(col+1)].horizontal;
                        botCenter = energyMatrix[(row)*imageHeight+(col+1)].horizontal;
                        botRight = energyMatrix[(row+1)*imageHeight+(col+1)].horizontal;

                        min = std::min(std::min(botLeft, botCenter), botRight);
                    }

                    col++;

                    if(min == botLeft)
                        row--;
                    else if(min == botRight)
                        row++;

                    seams[i].add(row,col,energyMatrix[row*imageHeight+col].horizontal);

                }

            }

            int minSeam = seams[0].cost;
            int minSeamIndex = 0;
            for (int i = 0; i < imageWidth; ++i)
            {
                if(seams[i].cost < minSeam)
                {
                    minSeam = seams[i].cost;
                    minSeamIndex = i;
                }
            }

            Seam *seamRef = &seams[minSeamIndex];
            std::vector< std::pair< std::vector<SlVector3>::iterator, std::vector<pixelEnergy>::iterator > > pxls(imageHeight);

            for (int i = seamRef->seam.size()-1; i >= 0; i--)
            {
                //std::cout << "ITER " << i << "/" << seamRef->seam.size()-1 << std::endl;
                int offset = seamRef->coordinates(i).first*imageHeight + seamRef->coordinates(i).second;
                //std::cout << "\t" << image[offset+1][0] << " " << image[offset+1][1] << " " << image[offset+1][2] << std::endl;

                for (int j = seamRef->coordinates(i).first; j < imageWidth-1; ++j)
                {
                    int tmpOffset = j*imageHeight + seamRef->coordinates(i).second;
                    image[tmpOffset][0] = image[tmpOffset+imageHeight][0];
                    image[tmpOffset][1] = image[tmpOffset+imageHeight][1];
                    image[tmpOffset][2] = image[tmpOffset+imageHeight][2];
                }

                //Uncomment this to show seam
                /*
                image[offset][0] = 100.0;
                image[offset][1] = 100.0;
                image[offset][2] = 100.0;
                //*/

                //image.erase(pxls[i].first);
                //energyMatrix.erase(pxls[i].second);

                //std::cout << "\t" << image[offset][0] << " " << image[offset][1] << " " << image[offset][2] << std::endl;
                energyMap(energyMatrix, image, seamRef->coordinates(i).first, seamRef->coordinates(i).second, imageWidth-1, imageHeight);
            }

            imageWidth--;
        }

        if(colsLeft)
        {
            colsLeft--;

            //*
            Seam seams[imageHeight];
            for (int i = 0; i < imageHeight; ++i)
            {
                int row = 0;
                int col = i;

                seams[i].add(row,col,energyMatrix[row*imageHeight+col].horizontal);
                while(row < imageWidth-1)
                {
                    double rightUp, rightCenter, rightDown, min;

                    if(col == 0)
                    {
                        rightUp = -1;
                        rightCenter = energyMatrix[(row+1)*imageHeight+(col)].horizontal;
                        rightDown = energyMatrix[(row+1)*imageHeight+(col+1)].horizontal;

                        min = std::min(rightCenter, rightDown);
                    }
                    else if(col == imageHeight)
                    {
                        rightUp = energyMatrix[(row+1)*imageHeight+(col-1)].horizontal;
                        rightCenter = energyMatrix[(row+1)*imageHeight+(col+1)].horizontal;
                        rightDown = -1;

                        min = std::min(rightCenter, rightUp);
                    }
                    else
                    {
                        rightUp = energyMatrix[(row+1)*imageHeight+(col-1)].horizontal;
                        rightCenter = energyMatrix[(row+1)*imageHeight+(col)].horizontal;
                        rightDown = energyMatrix[(row+1)*imageHeight+(col+1)].horizontal;

                        min = std::min(std::min(rightUp, rightCenter), rightDown);
                    }

                    row++;

                    if(min == rightUp)
                        col--;
                    else if(min == rightDown)
                        col++;

                    seams[i].add(row,col,energyMatrix[row*imageHeight+col].horizontal);

                }

            }

            int minSeam = seams[0].cost;
            int minSeamIndex = 0;
            for (int i = 0; i < imageHeight; ++i)
            {
                if(seams[i].cost < minSeam)
                {
                    minSeam = seams[i].cost;
                    minSeamIndex = i;
                }
            }

            Seam *seamRef = &seams[minSeamIndex];
            std::vector< std::pair< std::vector<SlVector3>::iterator, std::vector<pixelEnergy>::iterator > > pxls(imageHeight);

            for (int i = seamRef->seam.size()-1; i >= 0; i--)
            {
                //std::cout << "ITER " << i << "/" << seamRef->seam.size()-1 << std::endl;
                int offset = seamRef->coordinates(i).first*input.height() + seamRef->coordinates(i).second;
                //std::cout << "\t" << image[offset+1][0] << " " << image[offset+1][1] << " " << image[offset+1][2] << std::endl;

                //*
                for (int j = seamRef->coordinates(i).second; j < imageHeight-1; ++j)
                {
                    int tmpOffset = seamRef->coordinates(i).first*input.height() + j;
                    image[tmpOffset][0] = image[tmpOffset+1][0];
                    image[tmpOffset][1] = image[tmpOffset+1][1];
                    image[tmpOffset][2] = image[tmpOffset+1][2];
                }
                //*/

                //Uncomment this to show seam
                /*
                image[offset][0] = 100.0;
                image[offset][1] = 100.0;
                image[offset][2] = 100.0;
                //*/

                //image.erase(image.begin()+offset);
                //energyMatrix.erase(energyMatrix.begin()+offset);

                //std::cout << "\t" << image[offset][0] << " " << image[offset][1] << " " << image[offset][2] << std::endl;
                energyMap(energyMatrix, image, seamRef->coordinates(i).first, seamRef->coordinates(i).second, imageWidth-1, input.height());
            }

            imageHeight--;
            //*/
        }

        std::cout << rowsLeft << " " << colsLeft << " " 
            << 1 - (rowsLeft + colsLeft)/(float)((input.width() - atoi(argv[3])) + (input.height() - atoi(argv[4])))*100 
            << "%" <<  std::endl;

    }

    std::cout << atoi(argv[3]) << " " << imageWidth << " " << imageHeight << " " << image.size() << std::endl;

    CImg<double> output(atoi(argv[3]), atoi(argv[4]), input.depth(), input.spectrum(), 0);
    //CImg<double> output(atoi(argv[3]), atoi(argv[4]), input.depth(), input.spectrum(), 0);
    for (unsigned int i=0; i<imageWidth; i++) 
    {
        for (unsigned int j=0; j<imageHeight; j++) 
        {
            output(i, j, 0) = image[i*input.height()+j][0];
            output(i, j, 1) = image[i*input.height()+j][1];
            output(i, j, 2) = image[i*input.height()+j][2];
        }
    }
    CImg<double> rgb = output.LabtoRGB();

    if (strstr(argv[2], "png"))
        rgb.save_png(argv[2]);
    else if (strstr(argv[2], "jpg"))
        rgb.save_jpeg(argv[2]);

    return 0;
}
