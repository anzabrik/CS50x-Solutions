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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            BYTE tempblue = round(fminf((.272 * image[h][w].rgbtRed + .534 * image[h][w].rgbtGreen + .131 * image[h][w].rgbtBlue), 0xff));
            BYTE tempgreen = round(fminf((.349 * image[h][w].rgbtRed + .686 * image[h][w].rgbtGreen + .168 * image[h][w].rgbtBlue), 0xff));
            BYTE tempred = round(fminf((.393 * image[h][w].rgbtRed + .769 * image[h][w].rgbtGreen + .189 * image[h][w].rgbtBlue), 0xff));

            image[h][w].rgbtBlue =  tempblue;
            image[h][w].rgbtRed =  tempred;
            image[h][w].rgbtGreen =  tempgreen;
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