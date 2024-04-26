#include <math.h>
#include <stdio.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Take one pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int average = round((image[h][w].rgbtBlue + image[h][w].rgbtRed + image[h][w].rgbtGreen) / 3.0);
            image[h][w].rgbtBlue = average;
            image[h][w].rgbtRed  = average;
            image[h][w].rgbtGreen = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_original[height][width];

    // Take one pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_original[h][w] = image[h][w];
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = image_original[h][width - 1 - w];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_original[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_original[h][w] = image[h][w];
        }
    }

    // Take one pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float bluesum = 0;
            float greensum = 0;
            float redsum = 0;

            float box_elements_count = 0;
            // For this pixel, sum the pixels around it
            for (int hh = h - 1; hh < h + 2; hh++)
            {
                for (int ww = w - 1; ww < w + 2; ww++)
                {
                    //If this pixel exists on the image, sum its values
                    if (hh >= 0 && hh < height && ww >= 0 && ww < width)
                    {
                        bluesum += image_original[hh][ww].rgbtBlue;
                        greensum += image_original[hh][ww].rgbtGreen;
                        redsum  += image_original[hh][ww].rgbtRed;
                        box_elements_count += 1;
                    }
                }
            }
            // Find the average for the pixel[h][w]
            image[h][w].rgbtBlue = round(bluesum / box_elements_count);
            image[h][w].rgbtGreen  = round(greensum / box_elements_count);
            image[h][w].rgbtRed = round(redsum / box_elements_count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_original[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_original[h][w] = image[h][w];
        }
    }

    int Gx_multiplicators[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy_multiplicators[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // Take a single pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float bluesumGx  = 0, greensumGx = 0, redsumGx = 0;
            float bluesumGy  = 0, greensumGy = 0, redsumGy = 0;
            int Gxm = 0;
            int Gym = 0;
            // For this pixel, determine the pixels around it

            for (int hh = h - 1; hh < h + 2; hh++)
            {
                for (int ww = w - 1; ww < w + 2; ww++)
                {

                    {
                        //If this pixel exists on the image, work with its values
                        if (hh >= 0 && hh < height && ww >= 0 && ww < width)
                        {
                            bluesumGx += image_original[hh][ww].rgbtBlue * Gx_multiplicators[Gxm];
                            greensumGx += image_original[hh][ww].rgbtGreen * Gx_multiplicators[Gxm];
                            redsumGx  += image_original[hh][ww].rgbtRed * Gx_multiplicators[Gxm];
                        }
                    }
                    Gxm++;

                    {
                        //If this pixel exists on the image, work with its values
                        if (hh >= 0 && hh < height && ww >= 0 && ww < width)
                        {
                            bluesumGy += image_original[hh][ww].rgbtBlue * Gy_multiplicators[Gym];
                            greensumGy += image_original[hh][ww].rgbtGreen * Gy_multiplicators[Gym];
                            redsumGy  += image_original[hh][ww].rgbtRed * Gy_multiplicators[Gym];
                        }
                    }
                    Gym++;
                }
            }
            // combines Gx and Gy into a final value = square root of Gx^2 + Gy^2;
            image[h][w].rgbtBlue = round(fmin(sqrt((double)(bluesumGx * bluesumGx + bluesumGy * bluesumGy)), 0xff));
            image[h][w].rgbtGreen  = round(fmin(sqrt((double)(greensumGx * greensumGx + greensumGy * greensumGy)), 0xff));
            image[h][w].rgbtRed = round(fmin(sqrt((double)(redsumGx * redsumGx + redsumGy * redsumGy)), 0xff));
        }
    }
    return;
}
