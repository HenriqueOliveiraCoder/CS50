#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE mean;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            mean = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            image[i][j].rgbtRed = mean;
            image[i][j].rgbtGreen = mean;
            image[i][j].rgbtBlue = mean;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflected[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            reflected[i][j] = image[i][width - j - 1];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = reflected[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    int sumRed, sumGreen, sumBlue;
    float divider;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;
            divider = 0.0;

            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int newi = i + k;
                    int newj = j + l;

                    if (newi >= 0 && newi < height && newj >= 0 && newj < width)
                    {
                        sumRed += image[newi][newj].rgbtRed;
                        sumGreen += image[newi][newj].rgbtGreen;
                        sumBlue += image[newi][newj].rgbtBlue;
                        divider++;
                    }
                }
            }

            tempImage[i][j].rgbtRed = round(sumRed / divider);
            tempImage[i][j].rgbtGreen = round(sumGreen / divider);
            tempImage[i][j].rgbtBlue = round(sumBlue / divider);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tempImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = tempImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = tempImage[i][j].rgbtBlue;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sumRedGx, sumGreenGx, sumBlueGx, sumRedGy, sumGreenGy, sumBlueGy;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sumRedGx = 0;
            sumGreenGx = 0;
            sumBlueGx = 0;
            sumRedGy = 0;
            sumGreenGy = 0;
            sumBlueGy = 0;

            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int newi = i + k;
                    int newj = j + l;

                    if (newi >= 0 && newi < height && newj >= 0 && newj < width)
                    {
                        sumRedGx += image[newi][newj].rgbtRed * Gx[k + 1][l + 1];
                        sumGreenGx += image[newi][newj].rgbtGreen * Gx[k + 1][l + 1];
                        sumBlueGx += image[newi][newj].rgbtBlue * Gx[k + 1][l + 1];

                        sumRedGy += image[newi][newj].rgbtRed * Gy[k + 1][l + 1];
                        sumGreenGy += image[newi][newj].rgbtGreen * Gy[k + 1][l + 1];
                        sumBlueGy += image[newi][newj].rgbtBlue * Gy[k + 1][l + 1];
                    }
                }
            }

            tempImage[i][j].rgbtRed = fmin(255, fmax(0, round(sqrt(pow(sumRedGx, 2) + pow(sumRedGy, 2)))));
            tempImage[i][j].rgbtGreen = fmin(255, fmax(0, round(sqrt(pow(sumGreenGx, 2) + pow(sumGreenGy, 2)))));
            tempImage[i][j].rgbtBlue = fmin(255, fmax(0, round(sqrt(pow(sumBlueGx, 2) + pow(sumBlueGy, 2)))));
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tempImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = tempImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = tempImage[i][j].rgbtBlue;
        }
    }

    return;
}
