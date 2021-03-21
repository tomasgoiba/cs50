#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Struct for the sums of each color's values
typedef struct
{
    int rgbsRed;
    int rgbsGreen;
    int rgbsBlue;
}
RGBSUMS;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate over each pixel in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average value of red, green, and blue for pixel image[i][j]
            int rgbt_average = roundf((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float)3);

            // Assign the average value to each byte in the struct
            image[i][j].rgbtBlue = rgbt_average;
            image[i][j].rgbtGreen = rgbt_average;
            image[i][j].rgbtRed = rgbt_average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Calculate the iterations needed to reverse the pixels in each row, depending on whether 'width' is odd or even
    int reps;

    if (width % 2 == 1)
    {
        reps = (width - 1) / 2;
    }
    else
    {
        reps = width / 2;
    }

    // Iterate over every row in image[height]
    for (int i = 0; i < height; i++)
    {
        // Iterate over 'reps' many pixels in row image[i]
        for (int j = 0, rep_count = 0; rep_count != reps; j++)
        {
            // Swap pixels on horizontally opposite sides
            rep_count++;
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - rep_count];
            image[i][width - rep_count] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocate memory for tmp_image, a temporary RGBTRIPLE array to store all the new pixel values
    RGBTRIPLE(*tmp_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    // Iterate over each pixel in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize sums and valid pixel count to 0
            RGBSUMS sums = {0};
            float pixels = 0;

            // Iterate over each position in the 3x3 blur box
            for (int k = i - 1; k < i + 2; k++)
            {
                // Index must be within 'height' range
                if (k >= 0 && k < height)
                {
                    for (int l = j - 1; l < j + 2; l++)
                    {
                        // Index must be within 'width' range
                        if (l >= 0 && l < width)
                        {
                            // Add values to the sum of each color, and keep track of the number of valid pixels
                            sums.rgbsRed += image[k][l].rgbtRed;
                            sums.rgbsGreen += image[k][l].rgbtGreen;
                            sums.rgbsBlue += image[k][l].rgbtBlue;
                            pixels++;
                        }
                    }
                }
            }

            // Assign the average value for each color to pixel tmp_image[i][j]
            tmp_image[i][j].rgbtRed = roundf(sums.rgbsRed / pixels);
            tmp_image[i][j].rgbtGreen = roundf(sums.rgbsGreen / pixels);
            tmp_image[i][j].rgbtBlue = roundf(sums.rgbsBlue / pixels);
        }
    }

    // Transfer the new pixel values to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tmp_image[i][j].rgbtRed;
            image[i][j].rgbtGreen = tmp_image[i][j].rgbtGreen;
            image[i][j].rgbtBlue = tmp_image[i][j].rgbtBlue;
        }
    }

    // Free memory for tmp_image
    free(tmp_image);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocate memory for tmp_image, a temporary RGBTRIPLE array to store all the new pixel values
    RGBTRIPLE(*tmp_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    // Sobel convolution kernels
    int GX[3][3] =
    {
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1}
    };

    int GY[3][3] =
    {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
    };

    // Iterate over each pixel in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize Gx and Gy to 0 for all colors
            RGBSUMS gx = {0};
            RGBSUMS gy = {0};

            // Iterate over each position in the 3x3 grid of pixels and each value in either kernel
            for (int k = i - 1, m = 0; k < i + 2; k++, m++)
            {
                // Pixels out of 'height' range count as 0
                if (k >= 0 && k < height)
                {
                    for (int l = j - 1, n = 0; l < j + 2; l++, n++)
                    {
                        // Pixels out of 'width' range count as 0
                        if (l >= 0 && l < width)
                        {
                            // Add weighted color values to the Gx and Gy sums for each color
                            gx.rgbsRed += image[k][l].rgbtRed * GX[m][n];
                            gx.rgbsGreen += image[k][l].rgbtGreen * GX[m][n];
                            gx.rgbsBlue += image[k][l].rgbtBlue * GX[m][n];
                            gy.rgbsRed += image[k][l].rgbtRed * GY[m][n];
                            gy.rgbsGreen += image[k][l].rgbtGreen * GY[m][n];
                            gy.rgbsBlue += image[k][l].rgbtBlue * GY[m][n];
                        }
                    }
                }
            }

            // Calculate new values for each color, capping at 255
            if (roundf(sqrt(pow(gx.rgbsRed, 2) + pow(gy.rgbsRed, 2))) > 255)
            {
                tmp_image[i][j].rgbtRed = 255;
            }
            else
            {
                tmp_image[i][j].rgbtRed = roundf(sqrt(pow(gx.rgbsRed, 2) + pow(gy.rgbsRed, 2)));
            }

            if (roundf(sqrt(pow(gx.rgbsGreen, 2) + pow(gy.rgbsGreen, 2))) > 255)
            {
                tmp_image[i][j].rgbtGreen = 255;
            }
            else
            {
                tmp_image[i][j].rgbtGreen = roundf(sqrt(pow(gx.rgbsGreen, 2) + pow(gy.rgbsGreen, 2)));
            }

            if (roundf(sqrt(pow(gx.rgbsBlue, 2) + pow(gy.rgbsBlue, 2))) > 255)
            {
                tmp_image[i][j].rgbtBlue = 255;
            }
            else
            {
                tmp_image[i][j].rgbtBlue = roundf(sqrt(pow(gx.rgbsBlue, 2) + pow(gy.rgbsBlue, 2)));
            }
        }
    }

    // Transfer the new pixel values to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tmp_image[i][j].rgbtRed;
            image[i][j].rgbtGreen = tmp_image[i][j].rgbtGreen;
            image[i][j].rgbtBlue = tmp_image[i][j].rgbtBlue;
        }
    }

    // Free memory for tmp_image
    free(tmp_image);
    return;
}
