#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ppm_io.h"
#include "imageManip.h"

int main(int argc, char *argv[]){

  //checks if there are less than 3 arguments
  if (argc < 3) {
    if (argc == 2) {
      fprintf(stderr, "Failed to supply input filename or output filename, or both\n");
    }
    return 1;
  }

  //opens the input and output file and creates an array that will take in the userinput
  char func[100];
  FILE * input_file = fopen(argv[1], "rb");
  FILE * output_file = fopen (argv[2], "wb");

  //checks if input file is null
  if (input_file == NULL) {
    fprintf(stderr, "Specified input file could not be opened\n");
    return 2;
  }

  //checks if output file is null   
  if (output_file == NULL) {
    fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
    return 7;
  }

  //reads in the input file, and checks if input image is null
  Image * input = read_ppm(input_file);
  if (input == NULL) {
    fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    return 3;
  }

  //checks if number of arguments is less than 4
    if (argc < 4) {
    fclose(input_file);
    fclose(output_file);
    fprintf(stderr, "No operation name was specified, or operation name specified was invalid\n");
    return 4;
  }
    //checks if the function is exposure, and checks for # of arguments
  strcpy(func,argv[3]);
  if (strcmp(func,"exposure") == 0){
    if (argc != 5){
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }
    //checks if ev is between -3 and 3
    float ev = atof(argv[4]);
    if (ev>3 || ev < -3)
      return 6;
    //applies exposure to the image and checks if the output file can be opened for writing
    Image * new_image = exposure(input, ev);
    if (ferror(output_file) != 0){
      fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails");
      return 7;
    }
    write_ppm(output_file,new_image);
    free(input->data);
    free(input);
  }

  //checks if the function is blend, and checks for # of arguments  
  else if (strcmp(func,"blend") == 0){
    if (argc != 6){
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }
    //opens the second file for reading and check if the file or input image is null
    FILE * input_file2 = fopen(argv[4], "rb");
    if (input_file2 == NULL) {
    fprintf(stderr, "Specified input file could not be opened\n");
    return 2;
  }
    Image * input2 = read_ppm(input_file2);
    if (input2 == NULL) {
    fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    return 3;
  }
    //checks if value of alpha is inbetween 0 and 1
    float alpha = atof(argv[5]);
    if (alpha <= 0 || alpha >= 1)
      return 6;
    //applies blend to two images and checks if output file can be opened for writing
    Image * new_image = blend(input, alpha, input2);
    if (ferror(output_file) != 0){
      fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
      return 7;
    }
    write_ppm(output_file,new_image);
    fclose(input_file2);
    free(input->data);
    free(input);
    free(input2->data);
    free(input2);
    free(new_image->data);
    free(new_image);
  }
  //checks if the function is zoomin, and checks for # of arguments 
  else if (strcmp(func,"zoom_in") == 0){
    if (argc != 4){
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specifi\
ed operation\n");
      return 5;
    }
    //applies zoomin to image and checks if output file can be opened for writing 
    Image * new_image = zoom_in(input);
    if(write_ppm(output_file,new_image) < 0){
      fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
      return 7;
    }
    free(input->data);
    free(input);
    free(new_image->data);
    free(new_image);
  }  
  //checks if the function is zoomout, and checks for # of arguments 
  else if (strcmp(func,"zoom_out") == 0){
    if(argc != 4){
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specifi\
ed operation\n");
      return 5;
  }
    //applies zoomout to image and checks if output file can be opened for writing 
    Image * new_image =	zoom_out(input);
    if (ferror(output_file) != 0){
      fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
      return 7;
    }
    write_ppm(output_file,new_image);
    free(input->data);
    free(input);
    free(new_image->data);
    free(new_image);
  }
  //checks if the function is pointilism, and checks for # of arguments 
  else if (strcmp(func,"pointilism") == 0){
    if (argc != 4){
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specifi\
ed operation\n");
      return 5;
    }
    //applies pointilism to image and checks if output file can be opened for writing 
    Image * new_image = pointilism(input);
    if (ferror(output_file) != 0){
      fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails");
      return 7;
    }
    write_ppm(output_file,new_image);
    free(input->data);
    free(input);
    free(new_image->data);
    free(new_image);
  }
  //checks if the function is swirl, and checks for # of arguments 
  else if (strcmp(func,"swirl") == 0){
    if (argc != 7){
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specifi\
ed operation\n");
      return 5;
    }
    //checks to see if values of userinput is valid
    float val1 = atof(argv[4]);
    float val2 = atof(argv[5]);
    float val3 = atof(argv[6]);
    if (val1 <= 0 || val2 <= 0 || val3 <= 0) {
      fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
      return 6;
    }
    Image * new_image = swirl(input,val1,val2,val3);
    //checks if user_input is within the dimensions of the original image
    if (new_image == NULL) {
      fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
      return 6;
    }
    //applies swirl to image and checks if output file can be opened for writing 
    if (ferror(output_file) != 0){
      fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
      return 7;
    }
    write_ppm(output_file,new_image);
    free(input->data);
    free(input);
    free(new_image->data);
    free(new_image);
  }
  //checks if the function is blur, and checks for # of arguments
  else if (strcmp(func,"blur") == 0){
    if (argc != 5){
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specifi\
ed operation\n");
      return 5;
    }
    //check if the sigma values are valid
    float effect = atof(argv[4]);
    if (effect <= 0){
      fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
      return 6;
    }
    //applies blur to image and checks if output file can be opened for writing 
    Image * new_image = blur(input,effect);
    if (ferror(output_file) != 0){
      fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
      return 7;
    }
    write_ppm(output_file,new_image);
    free(input->data);
    free(input);
    free(new_image->data);
    free(new_image);
  }
  //if the function name and the username doesn't match, return 4
  else {
    fclose(input_file);
    fclose(output_file);
    fprintf(stderr, "No operation name was specified, or operation name specified was invalid\n");
    return 4;
  }
  // if everything goes smooth, finish the program after closing input and output file
  fclose(input_file);
  fclose(output_file);
  return 0;
  
}
