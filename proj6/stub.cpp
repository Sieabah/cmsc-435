/**
 * stub.cpp
 *
 * Written by Christopher Sidell
 *
 * This program is an implementation of the content aware image resizing
 *     algorithm
 */

#define cimg_display 0
#define debug_energy_map 0
#include "CImg.h"
#include "slVector.H"
#include "Seam.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace cimg_library;

/**
 * PixelEnergy is a way to represent a pixel's energy depending on the direction
 *     you're creating a seam from
 */
struct pixelEnergy
{
    //Constructor
    pixelEnergy(): horizontal(0), vertical(0)
    {}

    //Energy values
    double vertical;
    double horizontal;
};

/**
 * energyMap - Creates an energy map from the given pixel
 *     This creates a two-energy system in which the CAIR algorithm will 
 *         emphasize horizontal or vertical energy depending on the direction
 *         the seam is being created in
 * @param matrix energy cost matrix
 * @param image  the pixel array
 * @param i      row position
 * @param j      column position
 * @param width  image width
 * @param height image height
 */
void energyMap(std::vector<pixelEnergy> &matrix, std::vector<SlVector3> &image, 
    int i, int j, int width, int height)
{
    //Horizontal-seam energy calculation
    //
    //If row is on left edge
    if(j == 0)
        //Average current pixel with right pixel
        matrix[i*height+j].horizontal = mag(image[i*height+j] - image[i*height+j+1]);
    //If row is on right edge
    else if (j+1 >= height)
        //Average current pixel with left pixel
        matrix[i*height+j].horizontal = mag(image[i*height+j-1] - image[i*height+j]);
    else
        //Otherwise calculate surrounding pixels
        matrix[i*height+j].horizontal = mag(image[i*height+j-1] - image[i*height+j+1]);

    //Vertical-seam energy calculation
    //
    //If col is on left edge
    if(i == 0)
        matrix[i*height+j].vertical = mag(image[i*height+j] - image[(i+1)*height+j]);
    //If col is on right edge
    else if (i+1 >= width)
        matrix[i*height+j].vertical = mag(image[(i-1)*height+j] - image[i*height+j]);
    else
        matrix[i*height+j].vertical = mag(image[(i-1)*height+j] - image[(i+1)*height+j]);
}

/**
 * The main program
 */
int main(int argc, char *argv[]) {
    /*
    START GIVEN CODE
     */
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

    /*
    END GIVEN CODE
     */
    
    //Create an energy matrix to hold pixel energies
    std::vector<pixelEnergy> energyMatrix(input.width()*input.height());

    //Debug energy map output
    #if debug_energy_map
    std::vector<SlVector3> energyImageH(input.width()*input.height());
    std::vector<SlVector3> energyImageV(input.width()*input.height());
    #endif

    //Precalculate energy of all pixels in horizontal and vertical space
    for (unsigned int i=0; i<input.width(); i++) 
    {
        for (unsigned int j=0; j<input.height(); j++) 
        {
            //Send to energy map function
            energyMap(energyMatrix, image, i, j, input.width(), input.height());

            //Debug energy map creation
            #if debug_energy_map
            int offset = i*input.height()+j;
            energyImageH[offset][0] = energyMatrix[offset].horizontal;
            energyImageH[offset][1] = energyMatrix[offset].horizontal;
            energyImageH[offset][2] = energyMatrix[offset].horizontal;
            energyImageV[offset][0] = energyMatrix[offset].vertical;
            energyImageV[offset][1] = energyMatrix[offset].vertical;
            energyImageV[offset][2] = energyMatrix[offset].vertical;
            #endif
        }
    }

    //Debug energy map creation
    #if debug_energy_map
    CImg<double> energyOutput(input.width(), input.height(), input.depth(), 
        input.spectrum(), 0);
    //Based off code given at bottom of template
    for (unsigned int i=0; i<input.width(); i++) 
    {
        for (unsigned int j=0; j<input.height(); j++) 
        {
            energyOutput(i,j,0) = energyImageH[i*input.height()+j][0];
            energyOutput(i,j,1) = energyImageH[i*input.height()+j][1];
            energyOutput(i,j,2) = energyImageH[i*input.height()+j][2];
        }
    }

    energyOutput.save_png("energyH.png");
    for (unsigned int i=0; i<input.width(); i++) 
    {
        for (unsigned int j=0; j<input.height(); j++) 
        {
            energyOutput(i,j,0) = energyImageV[i*input.height()+j][0];
            energyOutput(i,j,1) = energyImageV[i*input.height()+j][1];
            energyOutput(i,j,2) = energyImageV[i*input.height()+j][2];
        }
    }

    energyOutput.save_png("energyV.png");
    #endif


    //Calculate how many rows/cols we're going to need to adjust by
    int rowsLeft = input.width() - atoi(argv[3]);
    int colsLeft = input.height() - atoi(argv[4]);

    //Create variables for the changing height/width as we work through image
    int imageWidth = input.width();
    int imageHeight = input.height();

    //While we still have rows/columns
    //The approach taken here is remove row, remove col, remove row...etc
    while(rowsLeft || colsLeft)
    {
        //VERTICAL SEAM
        if(rowsLeft)
        {  
            rowsLeft--;

            //Create an array of all possible seams, optimization is to keep 
            //  track as you go through seams of which is the smallest
            Seam seams[imageWidth];

            //For all pixels in width
            for (int i = 0; i < imageWidth; ++i)
            {
                //Create variables for shifting row/col
                int row = i;
                int col = 0;

                //Add initial point
                seams[i].add(row,col,
                    energyMatrix[row*imageHeight+col].vertical);

                //For height of image
                while(col < imageHeight-1)
                {
                    double botLeft, botCenter, botRight, min;

                    //For each case find minimal energy to on next pixel
                    //Left edge
                    if(row == 0)
                    {
                        /*
                        |o.
                        |xx
                         */
                        botLeft = -1;
                        botCenter = energyMatrix[(row)*imageHeight+(col+1)].vertical;
                        botRight = energyMatrix[(row+1)*imageHeight+(col+1)].vertical;

                        min = std::min(botCenter, botRight);
                    }
                    //Right edge
                    else if(row == imageWidth)
                    {
                        /*
                        .o|
                        xx|
                         */
                        botLeft = energyMatrix[(row-1)*imageHeight+(col+1)].vertical;
                        botCenter = energyMatrix[(row+1)*imageHeight+(col+1)].vertical;
                        botRight = -1;

                        min = std::min(botCenter, botLeft);
                    }
                    else
                    {
                        /*
                        .o.
                        xxx
                         */
                        botLeft = energyMatrix[(row-1)*imageHeight+(col+1)].vertical;
                        botCenter = energyMatrix[(row)*imageHeight+(col+1)].vertical;
                        botRight = energyMatrix[(row+1)*imageHeight+(col+1)].vertical;

                        min = std::min(std::min(botLeft, botCenter), botRight);
                    }

                    //Increment depth of the seam
                    col++;

                    //Determine if we've shifted 
                    if(min == botLeft)
                        row--;
                    else if(min == botRight)
                        row++;

                    seams[i].add(row,col,energyMatrix[row*imageHeight+col].vertical);

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

            //Create a reference for simplicity
            Seam *seamRef = &seams[minSeamIndex];

            //std::vector< std::pair< std::vector<SlVector3>::iterator, 
            //  std::vector<pixelEnergy>::iterator > > pxls(imageHeight);

            for (int i = seamRef->seam.size()-1; i >= 0; i--)
            {
                //Calculate offset
                int offset = seamRef->coordinates(i).first*imageHeight 
                    + seamRef->coordinates(i).second;
                
                //For all pixels right of seam pixel
                for (int j = seamRef->coordinates(i).first; j < imageWidth-1; ++j)
                {
                    //Copy data, essentially shifting all right pixels to the
                    //  left by 1
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

                //Possibly implement way to delete pixels instead of shifting
                //image.erase(pxls[i].first);
                //energyMatrix.erase(pxls[i].second);

                //Recalculate pixel power 
                energyMap(energyMatrix, image, seamRef->coordinates(i).first, 
                    seamRef->coordinates(i).second, imageWidth-1, imageHeight);
            }

            //Remove 1 from new image Width
            imageWidth--;
        }

        //HORIZONTAL SEAM
        if(colsLeft)
        {
            colsLeft--;

            //Preallocate seams
            Seam seams[imageHeight];

            //For all pixels in height of image
            for (int i = 0; i < imageHeight; ++i)
            {
                //Create variables for shifting seam
                int row = 0;
                int col = i;

                //Add starting pixel
                seams[i].add(row,col,energyMatrix[row*imageHeight+col].horizontal);
                while(row < imageWidth-1)
                {
                    double rightUp, rightCenter, rightDown, min;

                    //top edge
                    if(col == 0)
                    {
                        /*
                        ___
                        |ox
                        |.x
                         */
                        rightUp = -1;
                        rightCenter = energyMatrix[(row+1)*imageHeight+(col)].horizontal;
                        rightDown = energyMatrix[(row+1)*imageHeight+(col+1)].horizontal;

                        min = std::min(rightCenter, rightDown);
                    }
                    //bottom edge
                    else if(col == imageHeight)
                    {
                        /*
                        |.x
                        |ox
                        ___
                         */
                        rightUp = energyMatrix[(row+1)*imageHeight+(col-1)].horizontal;
                        rightCenter = energyMatrix[(row+1)*imageHeight+(col+1)].horizontal;
                        rightDown = -1;

                        min = std::min(rightCenter, rightUp);
                    }
                    else
                    {
                        /*
                        |.x
                        |ox
                        |.x
                         */
                        rightUp = energyMatrix[(row+1)*imageHeight+(col-1)].horizontal;
                        rightCenter = energyMatrix[(row+1)*imageHeight+(col)].horizontal;
                        rightDown = energyMatrix[(row+1)*imageHeight+(col+1)].horizontal;

                        min = std::min(std::min(rightUp, rightCenter), rightDown);
                    }

                    //Progress one row
                    row++;

                    //Figure out if we're going up or down in height
                    if(min == rightUp)
                        col--;
                    else if(min == rightDown)
                        col++;

                    //Add new location to seam
                    seams[i].add(row,col,energyMatrix[row*imageHeight+col].horizontal);
                }

            }

            //Find min seam cost, just a simple min of array search
            //  For better OO, you can put this off into a function
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

            //Create reference to seam
            Seam *seamRef = &seams[minSeamIndex];

            //std::vector< std::pair< std::vector<SlVector3>::iterator, 
            //    std::vector<pixelEnergy>::iterator > > pxls(imageHeight);

            for (int i = seamRef->seam.size()-1; i >= 0; i--)
            {
                int offset = seamRef->coordinates(i).first*input.height() 
                    + seamRef->coordinates(i).second;

                //For all pixels below current, shift them up
                for (int j = seamRef->coordinates(i).second; j < imageHeight-1; ++j)
                {
                    int tmpOffset = seamRef->coordinates(i).first*input.height() + j;
                    image[tmpOffset][0] = image[tmpOffset+1][0];
                    image[tmpOffset][1] = image[tmpOffset+1][1];
                    image[tmpOffset][2] = image[tmpOffset+1][2];
                }

                //image.erase(image.begin()+offset);
                //energyMatrix.erase(energyMatrix.begin()+offset);

                //recalculate power of pixel
                energyMap(energyMatrix, image, seamRef->coordinates(i).first, 
                    seamRef->coordinates(i).second, imageWidth-1, input.height());
            }

            //Decrement new image height
            imageHeight--;
        }

        //Output rowsLeft colsLeft and % completion
        std::cout << rowsLeft << " " << colsLeft << " " 
            <<(1 - (rowsLeft + colsLeft)/(float)((input.width() - atoi(argv[3])) 
                + (input.height() - atoi(argv[4]))))*100 
            << "%" <<  std::endl;
    }

    /*
    GIVEN OUTPUT CODE
     */
    CImg<double> output(atoi(argv[3]), atoi(argv[4]), input.depth(), input.spectrum(), 0);
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

    /*
    END OUTPUT CODE
     */
    return 0;
}
