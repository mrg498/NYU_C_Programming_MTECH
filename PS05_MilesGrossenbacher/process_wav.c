#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>


int usage(char* argv0)
{
  printf("Usage: %s -max amplitude infile.wav outfile.wav\n", argv0);
  printf("If no max value specified, make exact copy of .wav file.\n");
  return -1;
}

//main
int main(int argc, char* argv[])
{
  // if incorrect number of argv, print usage
  if(argc < 3){
    return usage(argv[0]);
  }

  //member variables
  FILE* ifp, * ofp;
  char* ifile, * ofile;
  int target_value = 0;
  bool max_flag = false;

  if(strcmp(argv[1], "-max") == 0){
    max_flag = true;
    target_value = atoi(argv[2]);
    ifile = argv[3];
    ofile = argv[4];
    //printf("%s\n", "first");
  }
  else{
    ifile = argv[1];
    ofile = argv[2];
    //printf("%s\n", "second");
  }

  printf("%s\n", ifile);
  printf("%s\n", ofile);

  // open source wav file
  if((ifp = fopen(ifile, "r")) == NULL){
    printf("%s\n", "Error: could not open wav file");
    return -1;
  }
  if((ofp = fopen(ofile, "w")) == NULL){
    printf("%s\n", "Error: could not open wav file");
    return -1;
  }

  unsigned char header[44];
  if (fread(header, sizeof(header), 1, ifp) != 1 ) {
    printf("Error: could not get header from wav file.");
    return -1;
  }

  char c;
  int c_counter = 0;
  for(int i = 0; i < 44; i++){
    c = isalpha(header[i]) ? header[i] : ' ';
    c_counter++;
    if(c_counter == 11){
      printf("\n");
      c_counter = 0;
    }
    printf("%c %02x", c, header[i]);
  }
    printf("\n" );
    int nchn = header[23]<<8 | header[22];
  	int fsamp = header[27]<<24 | header[26]<<16 | header[25]<<8 | header[24];
  	int bits_per_samp = header[35]<<8 | header[34];
  	int bytes_in_sound_data = header[43]<<24 | header[42]<<16 | header[41]<<8 | header[40];
  	printf("Number of channels: %d\n", nchn);
  	printf("Sampling Rate: %d\n", fsamp);
  	printf("Bits per audio word: %d\n", bits_per_samp);
  	printf("Bytes in sound data: %d\n", bytes_in_sound_data);

    int num_samples = (bytes_in_sound_data*8)/(bits_per_samp);
    printf("Number of samples -- %d\n", num_samples);
    short* x;
    if ((x = (short *)malloc(bytes_in_sound_data)) == NULL){
          // error reporting
          printf("%s\n", "Error: could not allocate memory for wav data");
          return -1;
    }

    if (fread(x,sizeof(*x), num_samples, ifp) != num_samples) {
          printf("%s\n", "Error: could not read wav data");
          return -1;
     }

     /*for(int i = 0; i< num_samples; i++){
       printf("%d, %d\n", i, x[i]);

     }*/
    int vmax = 0;
    for(int i = 0; i<num_samples; i++){
      if(abs(x[i]) > vmax){
        vmax = abs(x[i]);
      }
    }

    // if a max value was not entered, then just make the target value the max of the original
    if(!max_flag){
      target_value = vmax;
    }
    printf("Target max value -- %d\n", target_value);
    //print max absolute value
    printf("Original max value -- %d\n", vmax);
    int x_new;
    for(int i = 0; i<num_samples;i++){
      x_new = ((x[i]) * (target_value/(float)vmax));
      x[i] = x_new;
    }

    /*for(int i = 0; i< num_samples; i++){
      printf("%d, %d\n", i, x[i]);

    }*/

    fwrite(header, sizeof(header), 1, ofp);
    fwrite(x,sizeof(*x), num_samples, ofp);

    free(x);
    fclose(ifp);
    fclose(ofp);

  return 0;
}
