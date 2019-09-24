#include<stdio.h>
#include<string.h>
#include <stdlib.h>


int usage(char* arg0){
  printf("Usage: %s infile.txt", arg0);
  return -1;
}

int main(int argc, char* argv[]){
  //main variables
  FILE* fp;
  char* infile;
  char line [100];
  char* token;
  int token_count = 0;



  // check to make sure correct num argc
  if(argc < 2){
    usage(argv[0]);
  }

  // parse the name of the file
  infile = argv[1];

  // now try to open the file
  if ( (fp = fopen(infile, "r")) == NULL ) {
      fprintf(stderr, "ERROR: Cannot open %s\n", infile);
      return -1;
  }
  //printf("Opened the file!\n");

  //now read the file line by line
  while(fgets(line, 200, fp) != NULL){
    token = strtok(line, " \n");
    //printf("%s\n", token);
    token_count++;

    while ( (token = strtok(NULL, " \n") ) != NULL) {
      //printf("%s\n", token);
      token_count++;
    }
  }

  printf("%d\n", token_count);

  fclose(fp);

  return 0;

}
