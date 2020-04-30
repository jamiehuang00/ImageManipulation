#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H

Image * exposure(Image * image, float EV);
Image * blend(Image * image1,float alpha, Image * image2);
Image * zoom_in(Image * image);
Image * zoom_out();
Image * pointilism(Image * image);

Image * swirl(Image * image, double centerx, double centery, double scale);
Image * blur(Image * image, float sigma);
double ** gaussian(float sig);
Pixel blurOnePixel(Image * image, int x, int y, double ** filter, float sigma);

#endif
