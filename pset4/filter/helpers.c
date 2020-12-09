#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

float getKernelVal (int img[3][3], int kernel[3][3]);
int getPixelVal (float x, float y);
bool hasNeighbor (int h, int w, int height, int width, int neighbor);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    int avg;
    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++) {
        avg = round ((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0);
        image[h][w].rgbtRed = avg;
        image[h][w].rgbtGreen = avg;
        image[h][w].rgbtBlue = avg;
      }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE img[height][width];

    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++) {
        if (w >= width / 2) break;
        img[h][w] = image[h][w];
        image[h][w] = image[h][width - 1 - w];
        image [h][width - 1 - w] = img[h][w];
      }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    float denominator;
    RGBTRIPLE img[height][width];

    for (int h = 0; h < height; h++)
      for (int w = 0; w < width; w++)
        img[h][w] = image[h][w];

    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++) {

        denominator = 0;
        for (int i = 1; i <= 9; i++) denominator += (i == 5) ? 1 :
                                                    (hasNeighbor (h, w, height, width, i)) ? 1 : 0;

        image[h][w].rgbtRed = round (
          ((hasNeighbor (h, w, height, width, 1) ? img[h - 1][w - 1].rgbtRed : 0) // 1
         + (hasNeighbor (h, w, height, width, 2) ? img[h - 1][w].rgbtRed : 0) // 2
         + (hasNeighbor (h, w, height, width, 3) ? img[h - 1][w + 1].rgbtRed : 0) // 3
         + (hasNeighbor (h, w, height, width, 4) ? img[h][w - 1].rgbtRed : 0) // 4
         + image[h][w].rgbtRed // 5
         + (hasNeighbor (h, w, height, width, 6) ? img[h][w + 1].rgbtRed : 0) // 6
         + (hasNeighbor (h, w, height, width, 7) ? img[h + 1][w - 1].rgbtRed : 0) // 7
         + (hasNeighbor (h, w, height, width, 8) ? img[h + 1][w].rgbtRed : 0) // 8
         + (hasNeighbor (h, w, height, width, 9) ? img[h + 1][w + 1].rgbtRed : 0) // 9
          ) / denominator);

        image[h][w].rgbtGreen = round (
          ((hasNeighbor (h, w, height, width, 1) ? img[h - 1][w - 1].rgbtGreen : 0) // 1
         + (hasNeighbor (h, w, height, width, 2) ? img[h - 1][w].rgbtGreen : 0) // 2
         + (hasNeighbor (h, w, height, width, 3) ? img[h - 1][w + 1].rgbtGreen : 0) // 3
         + (hasNeighbor (h, w, height, width, 4) ? img[h][w - 1].rgbtGreen : 0) // 4
         + image[h][w].rgbtGreen // 5
         + (hasNeighbor (h, w, height, width, 6) ? img[h][w + 1].rgbtGreen : 0) // 6
         + (hasNeighbor (h, w, height, width, 7) ? img[h + 1][w - 1].rgbtGreen : 0) // 7
         + (hasNeighbor (h, w, height, width, 8) ? img[h + 1][w].rgbtGreen : 0) // 8
         + (hasNeighbor (h, w, height, width, 9) ? img[h + 1][w + 1].rgbtGreen : 0) // 9
          ) / denominator);

        image[h][w].rgbtBlue = round (
          ((hasNeighbor (h, w, height, width, 1) ? img[h - 1][w - 1].rgbtBlue : 0) // 1
         + (hasNeighbor (h, w, height, width, 2) ? img[h - 1][w].rgbtBlue : 0) // 2
         + (hasNeighbor (h, w, height, width, 3) ? img[h - 1][w + 1].rgbtBlue : 0) // 3
         + (hasNeighbor (h, w, height, width, 4) ? img[h][w - 1].rgbtBlue : 0) // 4
         + image[h][w].rgbtBlue // 5
         + (hasNeighbor (h, w, height, width, 6) ? img[h][w + 1].rgbtBlue : 0) // 6
         + (hasNeighbor (h, w, height, width, 7) ? img[h + 1][w - 1].rgbtBlue : 0) // 7
         + (hasNeighbor (h, w, height, width, 8) ? img[h + 1][w].rgbtBlue : 0) // 8
         + (hasNeighbor (h, w, height, width, 9) ? img[h + 1][w + 1].rgbtBlue : 0) // 9
          ) / denominator);

      }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    float denominator;
    RGBTRIPLE img[height][width];
    int Gx[3][3] = {
          {-1, 0, 1},
          {-2, 0, 2},
          {-1, 0, 1}
        },
        Gy[3][3] = {
          {-1, -2, -1},
          {0, 0, 0},
          {1, 2, 1}
        }, grid[3][3];

    for (int h = 0; h < height; h++)
      for (int w = 0; w < width; w++)
        img[h][w] = image[h][w];

    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++) {

        grid[0][0] = hasNeighbor (h, w, height, width, 1) ? img[h - 1][w - 1].rgbtRed : 0;
        grid[0][1] = hasNeighbor (h, w, height, width, 2) ? img[h - 1][w].rgbtRed : 0;
        grid[0][2] = hasNeighbor (h, w, height, width, 3) ? img[h - 1][w + 1].rgbtRed : 0;
        grid[1][0] = hasNeighbor (h, w, height, width, 4) ? img[h][w - 1].rgbtRed : 0;
        grid[1][1] = img[h][w].rgbtRed;
        grid[1][2] = hasNeighbor (h, w, height, width, 6) ? img[h][w + 1].rgbtRed : 0;
        grid[2][0] = hasNeighbor (h, w, height, width, 7) ? img[h + 1][w - 1].rgbtRed : 0;
        grid[2][1] = hasNeighbor (h, w, height, width, 8) ? img[h + 1][w].rgbtRed : 0;
        grid[2][2] = hasNeighbor (h, w, height, width, 9) ? img[h + 1][w + 1].rgbtRed : 0;

        image[h][w].rgbtRed = getPixelVal (getKernelVal (grid, Gx), getKernelVal (grid, Gy));

        grid[0][0] = hasNeighbor (h, w, height, width, 1) ? img[h - 1][w - 1].rgbtGreen : 0;
        grid[0][1] = hasNeighbor (h, w, height, width, 2) ? img[h - 1][w].rgbtGreen : 0;
        grid[0][2] = hasNeighbor (h, w, height, width, 3) ? img[h - 1][w + 1].rgbtGreen : 0;
        grid[1][0] = hasNeighbor (h, w, height, width, 4) ? img[h][w - 1].rgbtGreen : 0;
        grid[1][1] = img[h][w].rgbtGreen;
        grid[1][2] = hasNeighbor (h, w, height, width, 6) ? img[h][w + 1].rgbtGreen : 0;
        grid[2][0] = hasNeighbor (h, w, height, width, 7) ? img[h + 1][w - 1].rgbtGreen : 0;
        grid[2][1] = hasNeighbor (h, w, height, width, 8) ? img[h + 1][w].rgbtGreen : 0;
        grid[2][2] = hasNeighbor (h, w, height, width, 9) ? img[h + 1][w + 1].rgbtGreen : 0;

        image[h][w].rgbtGreen = getPixelVal (getKernelVal (grid, Gx), getKernelVal (grid, Gy));

        grid[0][0] = hasNeighbor (h, w, height, width, 1) ? img[h - 1][w - 1].rgbtBlue : 0;
        grid[0][1] = hasNeighbor (h, w, height, width, 2) ? img[h - 1][w].rgbtBlue : 0;
        grid[0][2] = hasNeighbor (h, w, height, width, 3) ? img[h - 1][w + 1].rgbtBlue : 0;
        grid[1][0] = hasNeighbor (h, w, height, width, 4) ? img[h][w - 1].rgbtBlue : 0;
        grid[1][1] = img[h][w].rgbtBlue;
        grid[1][2] = hasNeighbor (h, w, height, width, 6) ? img[h][w + 1].rgbtBlue : 0;
        grid[2][0] = hasNeighbor (h, w, height, width, 7) ? img[h + 1][w - 1].rgbtBlue : 0;
        grid[2][1] = hasNeighbor (h, w, height, width, 8) ? img[h + 1][w].rgbtBlue : 0;
        grid[2][2] = hasNeighbor (h, w, height, width, 9) ? img[h + 1][w + 1].rgbtBlue : 0;

        image[h][w].rgbtBlue = getPixelVal (getKernelVal (grid, Gx), getKernelVal (grid, Gy));

      }
    }

    return;
}


int getPixelVal (float x, float y) {
  int value = round (sqrt ((x * x) + (y * y)));

  return (value > 255) ? 255 : value;
}


float getKernelVal (int img[3][3], int kernel[3][3]) {
  float value = 0.0;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      value += img[i][j] * kernel[i][j];

  return value;

}


bool hasNeighbor (int h, int w, int height, int width, int neighbor) {
  return (neighbor == 1) ? ((h - 1 >= 0) && (w - 1 >= 0)) :
         (neighbor == 2) ? (h - 1 >= 0) :
         (neighbor == 3) ? ((h - 1 >= 0) && (w + 1 < width)) :
         (neighbor == 4) ? (w - 1 >= 0) :
         (neighbor == 6) ? (w + 1 < width) :
         (neighbor == 7) ? ((h + 1 < height) && (w - 1 >= 0)) :
         (neighbor == 8) ? (h + 1 < height) :
         (neighbor == 9) ? ((h + 1 < height) && (w + 1 < width)) : false;
}