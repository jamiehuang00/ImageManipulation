#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "ppm_io.h"
#include "imageManip.h"

//exposure takes in an image and a float as arguments
Image * exposure(Image * image, float EV) {
  //the total number of pixels is number of rows * number of columns
  int num_pixels = image->rows * image->cols;
  //pixels is assigned to be the image->data
  Pixel * pixels = image->data;
  //iterates through the total number of pixels
  for (int i = 0; i < num_pixels; i++) {
    //for all r, g, b values, checks to see if old value * 2^EV is larger than 255, if so, set it to 255
    //otherwise, set it to old value * 2^EV
    if (pixels[i].r*pow(2, EV) < 255) {
      pixels[i].r *= pow(2, EV);
    }
    else {
      pixels[i].r = 255;
    }
    if (pixels[i].b*pow(2, EV) < 255) {
      pixels[i].b *= pow(2, EV);
    }
    else {
      pixels[i].b = 255;
    }
    if (pixels[i].g*pow(2, EV) < 255) {
      pixels[i].g *= pow(2, EV);
    }
    else {
      pixels[i].g = 255;
    }
  }
  return image;
}

//blend takes in two images and an alpha value as parameters
Image * blend(Image * image1, float alpha, Image * image2) {
  int row1 = image1->rows;
  int row2 = image2->rows;
  int col1 = image1->cols;
  int col2 = image2->cols;
  int row;
  int col;
  int overrow;
  int overcol;
  Pixel * pixel1 = image1->data;
  Pixel * pixel2 = image2->data;
  //overrow and overcol are the parts that the two images overlap
  //row and col are the dimentions of the new image
  if (row1 > row2){
    overrow = row2;
    row = row1;
  }else if (row1 < row2){
    overrow = row1;
    row	= row2;
  }else{
    overrow = row1;
    row = row1;
  }
  if (col1 > col2){
    overcol = col2;
    col	= col1;
  }else if (col1 < col2){
    overcol = col1;
    col = col2;
  }else{
    overcol = col1;
    col	= col1;
  }
  //creates a new image that the pixels will go into
  Image * combined  = (Image *) malloc (sizeof(Image));
  combined->rows = row;
  combined->cols = col;
  combined->data = (Pixel*)malloc(sizeof(Pixel)*row*col);
  //iterate through the parts where the two images overlap
  for (int i = 0; i < overrow; i++){
    for (int j = 0; j < overcol; j++){
      combined->data[i*col+j].r = pixel1[i*col1+j].r*(alpha)+pixel2[i*col2+j].r*(1-alpha);
      combined->data[i*col+j].g = pixel1[i*col1+j].g*(alpha)+pixel2[i*col2+j].g*(1-alpha);
      combined->data[i*col+j].b = pixel1[i*col1+j].b*(alpha)+pixel2[i*col2+j].b*(1-alpha);
      }
  }
  //depending on cases where row1 and row2 are different and col1 and col2 are different
  //iterate through the parts that have bigger dimention and put in the rgb values of the original image
  if (row1 != row2 && col1 != col2 && row1 > row2){
    for (int i = overrow; i < row; i++){
      for (int j = 0; j < overcol; j++){
	combined->data[i*col+j].r = pixel1[i*col1+j].r;
        combined->data[i*col+j].g = pixel1[i*col1+j].g;
        combined->data[i*col+j].b = pixel1[i*col1+j].b;
      }
    }
    for (int i = 0; i < overrow; i++){
      for (int j = overcol; j < col; j++){
	combined->data[i*col+j].r = pixel2[i*col1+j].r;
        combined->data[i*col+j].g = pixel2[i*col1+j].g;
        combined->data[i*col+j].b = pixel2[i*col1+j].b;
      }
    }
    for (int i = overrow; i < row; i++){
      for (int j = overcol; j < col; j++){
	combined->data[i*col+j].r = 0;
        combined->data[i*col+j].g = 0;
        combined->data[i*col+j].b = 0;
      }
    }
  }
    if (row1 != row2 && col1 != col2 && row1 < row2){
      for (int i = overrow; i < row; i++){
	for (int j = 0; j < overcol; j++){
	  combined->data[i*col+j].r = pixel2[i*col2+j].r;
	  combined->data[i*col+j].g = pixel2[i*col2+j].g;
	  combined->data[i*col+j].b = pixel2[i*col2+j].b;
	}
      }
      for (int i = 0; i < overrow; i++){
	for (int j = overcol; j < col; j++){
	  combined->data[i*col+j].r = pixel1[i*col1+j].r;
	  combined->data[i*col+j].g = pixel1[i*col1+j].g;
	  combined->data[i*col+j].b = pixel1[i*col1+j].b;
	}
      }
      for (int i = overrow; i < row; i++){
	for (int j = overcol; j < col; j++){
	  combined->data[i*col+j].r = 0;
	  combined->data[i*col+j].g = 0;
	  combined->data[i*col+j].b = 0;
	}
      }
    }
    //case where the rows are the same but cols are different
      if (row1 == row2 && col1 != col2 && col1 < col2){
	for (int i = 0; i < overrow; i++){
	  for (int j = overcol; j < col; j++){
	    combined->data[i*col+j].r = pixel2[i*col2+j].r;
	    combined->data[i*col+j].g = pixel2[i*col2+j].g;
	    combined->data[i*col+j].b = pixel2[i*col2+j].b;
	  }
	}
      }
    if (row1 == row2 && col1 != col2 && col1 > col2){
      for (int i = 0; i < overrow; i++){
        for (int j = overcol; j < col; j++){
        combined->data[i*col+j].r = pixel1[i*col1+j].r;
        combined->data[i*col+j].g = pixel1[i*col1+j].g;
        combined->data[i*col+j].b = pixel1[i*col1+j].b;
        }
      }
    }
    //case where rows are different and cols are the same
    if (row1 != row2 && col1 == col2 && row1 < row2){
      for (int i = 0; i < overrow; i++){
        for (int j = overcol; j < col; j++){
        combined->data[i*col+j].r = pixel2[i*col2+j].r;
        combined->data[i*col+j].g = pixel2[i*col2+j].g;
        combined->data[i*col+j].b = pixel2[i*col2+j].b;
        }
      }
    }
    if (row1 != row2 && col1 == col2 && row1 > row2){
      for (int i = 0; i < overrow; i++){
        for (int j = overcol; j < col; j++){
        combined->data[i*col+j].r = pixel1[i*col1+j].r;
        combined->data[i*col+j].g = pixel1[i*col1+j].g;
        combined->data[i*col+j].b = pixel1[i*col1+j].b;
        }
      }
    }
    return combined;
}

//zoom_out takes in an image as parameter
Image * zoom_out(Image * image) {
  //assigns pixels to be the image->data, and declares new_rows to be old_rows/2 and new_cols to be old_cols/2
  Pixel * pixels = image->data;
  int old_rows = image->rows;
  int old_cols = image->cols;
  int new_rows = old_rows / 2;
  int new_cols = old_cols / 2;
  //assigns output to be the malloc with new_cols*new_cols
  Pixel * output = (Pixel*)malloc(sizeof(Pixel)*new_cols*new_rows);
  //loops through new_rows and new_cols, and for each pixel in the output, it is the average of the pixels in the 2x2 pixels in the old pixel array
  for (int i = 0; i < new_rows; i++){
    for (int j = 0; j < new_cols; j++) {
      output[i*new_cols+j].r = (pixels[i*2*old_cols+j*2].r+pixels[(i*2+1)*old_cols+j*2].r+pixels[i*2*old_cols+j*2+1].r+pixels[(i*2+1)*old_cols+j*2+1].r)/4;
      output[i*new_cols+j].g = (pixels[i*2*old_cols+j*2].g+pixels[(i*2+1)*old_cols+j*2].g+pixels[i*2*old_cols+j*2+1].g+pixels[(i*2+1)*old_cols+j*2+1].g)/4;
      output[i*new_cols+j].b = (pixels[i*2*old_cols+j*2].b+pixels[(i*2+1)*old_cols+j*2].b+pixels[i*2*old_cols+j*2+1].b+pixels[(i*2+1)*old_cols+j*2+1].b)/4;
    }
  }
  //mallocs a new image, and assigns the im->data to be output
  Image * im = (Image*)malloc(sizeof(Image));
  im->cols = new_cols;
  im->rows = new_rows;
  im->data = output;
  return im;
}

//zoom_in takes in an image as a parameter
Image * zoom_in(Image * image) {
  //assigns pixels to be image->data, and new_rows and new_cols to be 2 times old_rows and old_cols
  Pixel * pixels = image->data;
  int old_rows = image->rows;
  int old_cols = image->cols;
  int new_rows = old_rows * 2;
  int new_cols = old_cols * 2;
  //mallocs an output array to be new_cols*old_cols
  Pixel * output = (Pixel*)malloc(sizeof(Pixel)*new_cols*new_rows);
  //loops through old_rows and old_cols and assigns pixels of output to be 2* pixels of input
  for (int i = 0; i < old_rows; i++){
    for (int j = 0; j < old_cols; j++) {
      output[2*i*new_cols+2*j] = pixels[i*old_cols+j];
      output[2*i*new_cols+2*j+1] = pixels[i*old_cols+j];
      output[(2*i+1)*new_cols+2*j] = pixels[i*old_cols+j];
      output[(2*i+1)*new_cols+2*j+1] = pixels[i*old_cols+j];
    }
  }
  //assigns im to be the malloced image
  Image * im = (Image*)malloc(sizeof(Image));
  im->cols = new_cols;
  im->rows = new_rows;
  im->data = output;
  return im;
}

//this is the pointilism function
Image * pointilism(Image * image) {
  Pixel * pixels = image->data;
  //total is the number of pixels and change is 3% of that to be iterated through for pointilism function
  int row = image->rows;
  int col = image->cols;
  int total = row*col;
  int change = total*0.03;
  // rgb values to be used for pointilism
  unsigned char r;
  unsigned char g;
  unsigned char b;
  //create new image called im that we will return
  Image * im = (Image*)malloc(sizeof(Image));
  im->cols = col;
  im->rows = row;
  im->data = (Pixel*)malloc(sizeof(Pixel)*col*row);
  //copy the original image into im 
  memcpy(im->data,pixels,sizeof(Pixel)*col*row);
  //iterate by the 3% times and get a random value of x,y and radius
  for (int i = 0; i<change; i++){
    int x = rand() % col;
    int y = rand() % row;
    int rad = rand() % 5 + 1;
    r = im->data[y*col+x].r;
    g = im->data[y*col+x].g;
    b = im->data[y*col+x].b;
    //iterate over the +- values of radius for both x and y and check if the values of x and y are within the range of the circle
    for (int j = (x-rad); j<=(x+rad);j++){
      for (int k = (y-rad); k <= (y+rad); k++){
	if (j>=0 && k >= 0 && j <col && k<row){
	  if (pow(x-j,2)+pow(y-k,2)<= pow(rad,2)){
	    im->data[k*col+j].r = r;
	    im->data[k*col+j].g = g;
	    im->data[k*col+j].b = b;
	  }
	}
      }
    }
  }
  return im;
}
//swirl takes in the image as well as x,y and scale values as its parameters
Image * swirl(Image * image, double centerx, double centery, double scale) {
  int row = image->rows;
  int col = image->cols;
  // if the userinputs are not valid, return NULL to return 6 in project.c
  if (scale < 0 || centerx < 0 || centerx >= col || centery < 0 || centery >= row) {
    return NULL;
  }
  // create new image called im to be storing pixel data after swirl
  Image * im = (Image*)malloc(sizeof(Image));
  im->cols = col;
  im->rows = row;
  im->data = (Pixel*)calloc(sizeof(Pixel),col*row);
  //iterate through all pixels
  for (int x = 0; x < col; x++){
    for (int y = 0; y < row; y++){
      double difx = x-centerx;
      double dify = y-centery;
      double changex = difx*difx;
      double changey = dify*dify;
      double sqr = sqrt(changex+changey);
      double alpha = sqr/scale;
      //orgx and orgy are the x and y values in the original image
      int orgx = ((difx*cos(alpha))-(dify*sin(alpha))+centerx);
      int orgy = ((difx*sin(alpha))+(dify*cos(alpha))+centery);
      if (orgx <col && orgy < row && orgx >=0 && orgy >= 0){
	im->data[y*col+x].r = image->data[orgy*col+orgx].r;
	im->data[y*col+x].g = image->data[orgy*col+orgx].g;
	im->data[y*col+x].b = image->data[orgy*col+orgx].b;
      }//if orgx or orgy is bigger than the original dimension or is less than 0, make the pixel black
      else{
	im->data[y*col+x].r = 0;
	im->data[y*col+x].g = 0;
	im->data[y*col+x].b = 0;
      }
    }
  }
  return im;
}

//blur takes in an image and sigma value as it's parameters
Image * blur(Image * image, float sigma) {
  //the simga value*10 is dim, and if dim is even then you add 1 to dim
  double dim = sigma*10;
  if ((int)dim % 2 == 0) {
    dim++;
  }
  //malloc output
  Pixel * output = malloc(sizeof(Pixel)*image->rows*image->cols);
  //assigns filter to be the applied gaussian, loops through rows and cols and applies blurOnePixel to all the pixels
  double ** filter = gaussian(sigma);
  for (int j = 0; j < image->rows; j++) {
    for (int i = 0; i < image->cols; i++) {
      int index = j*image->cols + i;
      output[index] = blurOnePixel(image, i, j, filter, sigma);
    }
  }
  //mallocs image, and assigns cols and rows
  Image * im = (Image*)malloc(sizeof(Image));
  im->cols = image->cols;
  im->rows = image->rows;
  im->data = output;
  //deallocates every index in filter
  for (int i = 0; i < dim; i++) {
    free(filter[i]);
  }
  //deallocates filter
  free(filter);
  return im;
}

//purpose of gaussian is to be helper function that when given a sigma value, returns a gaussian filter
//gaussian takes in a sig as parameter
double ** gaussian(float sig) {
  //dim is sig*10, if dim is even, then you add 1 to dim
  double PI = 3.14159265358979323846;
  double dim = sig*10;
  
  if ((int)dim % 2 == 0) {
    dim++;
  }

  //mallocs a matrix that is the dimension
  double ** matrix = (double **)malloc(sizeof(double*) * dim);

  //loops through dim, and assigns each index of matrix to sizeofdouble
  for (int i = 0; i < dim; i++){
    matrix[i] = (double*)malloc(dim * sizeof(double));
  }

  //loops through dim-1/2 twice for dx and dy
  for (int dx = 0; dx <= ((int)dim-1)/2; dx++) {
    for (int dy = 0; dy <= ((int)dim-1)/2; dy++) {
      //assigns g to be calculated from the formula on the project page
      double g = (1.0 / (2.0 * PI *pow(sig, 2))) * exp(-(pow(dx, 2) + pow(dy, 2)) / (2 * pow(sig, 2)));
      //assigns matrix of index to be g
      matrix[((int)dim-1)/2-dx][((int)dim-1)/2-dy] = g;
      matrix[((int)dim-1)/2-dx][((int)dim-1)/2+dy] = g;
      matrix[((int)dim-1)/2+dx][((int)dim-1)/2-dy] = g;
      matrix[((int)dim-1)/2+dx][((int)dim-1)/2+dy] = g;
    }
  }

   return matrix;
}

//blurOnePixel is another helper function and applies the gaussian filter to one pixel
//it takes in an image, x, y, a filter array, and sigma as it's parameters
Pixel blurOnePixel(Image * im, int x, int y, double ** filter, float sigma) {
  //assigns dim to be sigma*10, if dim is even, then you add one
  double dim = sigma*10;
  if ((int)dim%2 == 0) {
    dim++;
  }
  //assigns data to be im->data, and p to be dim/2, and initiates r, g, b values to be 0
  Pixel * data = im->data;
  Pixel pixel;
  int p = dim/2;
  double r, g, b;
  double sum = 0;
  r = 0;
  g = 0;
  b = 0;
  //loops from -p to p, and assigns r, g, b to be the dot product of data and gauss filter
  for (int dx = -p; dx <= p; dx++) {
    for (int dy = -p; dy <= p; dy++) {
      //if it's within range, then assign r, g, b values to be the dot product, calculates sum for later for normalization
      if (x+dx >= 0 && x+dx < im->cols && y+dy >= 0 && y+dy < im->rows) {
	r += data[(y+dy)*im->cols+x+dx].r*filter[p+dy][p+dx];
	g += data[(y+dy)*im->cols+x+dx].g*filter[p+dy][p+dx];
	b += data[(y+dy)*im->cols+x+dx].b*filter[p+dy][p+dx];
	sum += filter[p+dy][p+dx];
      }
    }

    //if r/sum, g/sum, and b/sum is bigger than 255, then we set pixel.r is 255.
  }
  if (r/sum > 255) {
    pixel.r = 255;
  }
  else {
    pixel.r = r/sum;
  }
  if (g/sum > 255) {
    pixel.g = 255;
  }
  else {
    pixel.g = g/sum;
  }
  if (b/sum > 255) {
    pixel.b = 255;
  }
  else {
    pixel.b = b/sum;
  }
  return pixel;
}

