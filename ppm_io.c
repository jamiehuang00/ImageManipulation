//Jamie Huang, jhuan131
//Ryunosuke Saito, rsaito1

// __Add your name and JHED above__
// ppm_io.c
// 601.220, Spring 2019
// Starter code for midterm project - feel free to edit/add to this file

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ppm_io.h"
#include "imageManip.h"


/* Read a PPM-formatted image from a file (assumes fp != NULL).
1;95;0c * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  Image * res = malloc(sizeof(Image)); //malloc the resulting image
  char buff[20];
  char tag[10];
  int rows;
  int cols;
  int colors;
  char comment;

  //scans the tag, p6, checks if it's null
  if (fgets(buff, sizeof(buff), fp) == 0) {
    printf("Tag is null");
  }
  //scans the first value in the ppm file and see if it is P6
  sscanf(buff, "%s", tag);
  if (tag_checker(tag) == 3) {
    return NULL;
  }//until you get to the first non-whitespace character, ignore whitespace
  while(isspace(comment = fgetc(fp))){
  }
  //if the next character is #, then it is a comment so read everthing until new line.
  if (comment == '#'){
    while (comment != '\n'){
      comment = fgetc(fp);
    } //if it's not # then put it back to be read for cols and rows
  } else {
    ungetc(comment,fp);
  }
  //scans the columns, rows, and color
  fgets(buff, sizeof(buff), fp);
  sscanf(buff, "%d %d", &cols, &rows);
  res->rows = rows;
  res->cols = cols;
  res->data = (Pixel *)malloc(rows*cols*sizeof(Pixel));


  //scans the color if it is 255
  fgets(buff, sizeof(buff), fp);
  sscanf(buff, "%d", &colors);
  if (color_checker(colors) == 3) {
    return NULL;
  }

  //checks the values of rows and cols to be bigger than 0
  if (size_checker (rows, cols) == 3) {
    return NULL;
  }

  //reads in the data
  fread(res->data, sizeof(Pixel), rows*cols, fp);
  return res;
  
}

//checks if the rows and columns are bigger than 0
int size_checker(int row, int col) {
  if (row < 0 || col < 0) {
    return 3;
  }
  else {
    return 0;
  }
}

//checks if the tag is P6
int tag_checker (char tag[]){
  if (strcmp(tag, "P6") != 0)
    return 3;
  else
    return 0;
}

//checks if the color has 255 pixels
int color_checker(int color){
  if (color != 255)
    return 3;
  else
    return 0;
}

/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->cols * im->rows, fp);

  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }

  return num_pixels_written;
}

