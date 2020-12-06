#include "helpers.h"
#include <stdio.h>
#include <math.h>

int pixelmax(int x);

// Convert image to grayscale - DONE
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through all pixels, calculate average and set new greys
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Set at least one of the ints to floats to avoid truncation after division.
            int greyavg = round(((float) image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = greyavg;
            image[i][j].rgbtGreen = greyavg;
            image[i][j].rgbtRed = greyavg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            int tmpBlue = image[i][j].rgbtBlue;
            int tmpGreen = image[i][j].rgbtGreen;
            int tmpRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;

            image[i][width - 1 - j].rgbtBlue = tmpBlue;
            image[i][width - 1 - j].rgbtGreen = tmpGreen;
            image[i][width - 1 - j].rgbtRed = tmpRed;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through all pixels, make an image copy
    RGBTRIPLE imagecopy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagecopy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            imagecopy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            imagecopy[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }

    // Box blur. Use values in imagecopy to modify original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int neighbours = 0;
            float Blue = 0;
            float Green = 0;
            float Red = 0;

            if (i - 1 >= 0 && j - 1 >= 0)
            {
                // collect top left
                Blue += imagecopy[i - 1][j - 1].rgbtBlue;
                Green += imagecopy[i - 1][j - 1].rgbtGreen;
                Red += imagecopy[i - 1][j - 1].rgbtRed;
                neighbours++;
            }

            if (i - 1 >= 0)
            {
                // collect top
                Blue += imagecopy[i - 1][j].rgbtBlue;
                Green += imagecopy[i - 1][j].rgbtGreen;
                Red += imagecopy[i - 1][j].rgbtRed;
                neighbours++;
            }

            if (i - 1 >= 0 && j + 1 <= width - 1)
            {
                // collect top right
                Blue += imagecopy[i - 1][j + 1].rgbtBlue;
                Green += imagecopy[i - 1][j + 1].rgbtGreen;
                Red += imagecopy[i - 1][j + 1].rgbtRed;
                neighbours++;
            }

            if (j - 1 >= 0)
            {
                // collect middle left
                Blue += imagecopy[i][j - 1].rgbtBlue;
                Green += imagecopy[i][j - 1].rgbtGreen;
                Red += imagecopy[i][j - 1].rgbtRed;
                neighbours++;
            }

            // collect center
            Blue += imagecopy[i][j].rgbtBlue;
            Green += imagecopy[i][j].rgbtGreen;
            Red += imagecopy[i][j].rgbtRed;
            neighbours++;

            if (j + 1 <= width - 1)
            {
                // collect middle right
                Blue += imagecopy[i][j + 1].rgbtBlue;
                Green += imagecopy[i][j + 1].rgbtGreen;
                Red += imagecopy[i][j + 1].rgbtRed;
                neighbours++;
            }

            if (i + 1 <= height - 1 && j - 1 >= 0)
            {
                // collect bottom left
                Blue += imagecopy[i + 1][j - 1].rgbtBlue;
                Green += imagecopy[i + 1][j - 1].rgbtGreen;
                Red += imagecopy[i + 1][j - 1].rgbtRed;
                neighbours++;
            }

            if (i + 1 <= height - 1)
            {
                // collect bottom middle
                Blue += imagecopy[i + 1][j].rgbtBlue;
                Green += imagecopy[i + 1][j].rgbtGreen;
                Red += imagecopy[i + 1][j].rgbtRed;
                neighbours++;
            }

            if (i + 1 <= height - 1 && j + 1 <= width - 1)
            {
                // collect bottom right
                Blue += imagecopy[i + 1][j + 1].rgbtBlue;
                Green += imagecopy[i + 1][j + 1].rgbtGreen;
                Red += imagecopy[i + 1][j + 1].rgbtRed;
                neighbours++;
            }

            // Take averages and insert into original image

            image[i][j].rgbtBlue = round(Blue / neighbours);
            image[i][j].rgbtGreen = round(Green / neighbours);
            image[i][j].rgbtRed = round(Red / neighbours);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through all pixels, make an image copy
    RGBTRIPLE imagecopy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagecopy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            imagecopy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            imagecopy[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }
    
    // Calculate all Gx and Gy values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float GxBlue = 0;
            float GxGreen = 0;
            float GxRed = 0;
            float GyBlue = 0;
            float GyGreen = 0;
            float GyRed = 0;

            if (i - 1 >= 0 && j - 1 >= 0)
            {
                // collect top left Gx
                GxBlue += imagecopy[i - 1][j - 1].rgbtBlue * -1;
                GxGreen += imagecopy[i - 1][j - 1].rgbtGreen * -1;
                GxRed += imagecopy[i - 1][j - 1].rgbtRed * -1;
                
                // collect top left Gy
                GyBlue += imagecopy[i - 1][j - 1].rgbtBlue * -1;
                GyGreen += imagecopy[i - 1][j - 1].rgbtGreen * -1;
                GyRed += imagecopy[i - 1][j - 1].rgbtRed * -1;
                
            }

            if (i - 1 >= 0)
            {
                // collect top middle Gy
                GyBlue += imagecopy[i - 1][j].rgbtBlue * -2;
                GyGreen += imagecopy[i - 1][j].rgbtGreen * -2;
                GyRed += imagecopy[i - 1][j].rgbtRed * -2;
            }

            if (i - 1 >= 0 && j + 1 <= width - 1)
            {
                // collect top right Gx
                GxBlue += imagecopy[i - 1][j + 1].rgbtBlue;
                GxGreen += imagecopy[i - 1][j + 1].rgbtGreen;
                GxRed += imagecopy[i - 1][j + 1].rgbtRed;
                
                // collect top right Gy
                GyBlue += imagecopy[i - 1][j + 1].rgbtBlue * -1;
                GyGreen += imagecopy[i - 1][j + 1].rgbtGreen * -1;
                GyRed += imagecopy[i - 1][j + 1].rgbtRed * -1;
            }

            if (j - 1 >= 0)
            {
                // collect middle left Gx
                GxBlue += imagecopy[i][j - 1].rgbtBlue * -2;
                GxGreen += imagecopy[i][j - 1].rgbtGreen * -2;
                GxRed += imagecopy[i][j - 1].rgbtRed * -2;
            }

            if (j + 1 <= width - 1)
            {
                // collect middle right Gx
                GxBlue += imagecopy[i][j + 1].rgbtBlue * 2;
                GxGreen += imagecopy[i][j + 1].rgbtGreen * 2;
                GxRed += imagecopy[i][j + 1].rgbtRed * 2;
            }

            if (i + 1 <= height - 1 && j - 1 >= 0)
            {
                // collect bottom left Gx
                GxBlue += imagecopy[i + 1][j - 1].rgbtBlue * -1;
                GxGreen += imagecopy[i + 1][j - 1].rgbtGreen * -1;
                GxRed += imagecopy[i + 1][j - 1].rgbtRed * -1;
                
                // collect bottom left Gy
                GyBlue += imagecopy[i + 1][j - 1].rgbtBlue;
                GyGreen += imagecopy[i + 1][j - 1].rgbtGreen;
                GyRed += imagecopy[i + 1][j - 1].rgbtRed;
            }

            if (i + 1 <= height - 1)
            {
                // collect bottom middle Gy
                GyBlue += imagecopy[i + 1][j].rgbtBlue * 2;
                GyGreen += imagecopy[i + 1][j].rgbtGreen * 2;
                GyRed += imagecopy[i + 1][j].rgbtRed * 2;
            }

            if (i + 1 <= height - 1 && j + 1 <= width - 1)
            {
                // collect bottom right Gx
                GxBlue += imagecopy[i + 1][j + 1].rgbtBlue;
                GxGreen += imagecopy[i + 1][j + 1].rgbtGreen;
                GxRed += imagecopy[i + 1][j + 1].rgbtRed;
                
                // collect bottom right Gy
                GyBlue += imagecopy[i + 1][j + 1].rgbtBlue;
                GyGreen += imagecopy[i + 1][j + 1].rgbtGreen;
                GyRed += imagecopy[i + 1][j + 1].rgbtRed;
            }

            // Calculate final value of Gx with Gy - Sobel operator
            image[i][j].rgbtBlue = pixelmax(round(sqrt(pow(GxBlue, 2) + pow(GyBlue, 2))));
            image[i][j].rgbtGreen = pixelmax(round(sqrt(pow(GxGreen, 2) + pow(GyGreen, 2))));
            image[i][j].rgbtRed = pixelmax(round(sqrt(pow(GxRed, 2) + pow(GyRed, 2))));
        }
    }    
    return;
}

// cap max int for a pixel at 255
int pixelmax(int x)
{
    if (x < 255)
    {
        return x;
    }
    
    else
    {
        return 255;
    }
}