#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum {
    M_ENC = 1,
    M_DEC
};
#define LINE_LEN 80

bool keywordValid(char *input);
void enc_dec(char *key, char *string, char *encodedString, int mode);
char shift(char ch, int shiftVal, int mode);

//usage function
void usage(char * argv0) {
    fprintf(stderr, "Usage: %s [-e|-d] keyWord input_file.txt output_file.txt\n", argv0);
}

//beginning of main ///////////////////////
int main(int argc, char * argv[])
{
    int i, mode;
    char *keyWord, istr[LINE_LEN], ostr[LINE_LEN];
    char *ifile, *ofile;
    FILE *ifp, *ofp;

    /* set defaults */
    mode = M_ENC;

    // if there is not the correct number of command line arguments, or the options are not put in correctly
    // return and print usage
    int min_num_args = 4;
    int max_num_args = 5;
    if(argc < min_num_args || argc > max_num_args){
      usage(argv[0]);
      return -1;
    }

    /* parse command line and open files */
    //add code here

    // check if the first char from the argv[1] is '-', this means a mode is chosen
    // so we figure out which one by checking the second character of argv[1]
    i = 1;
    while (argv[i][0] == '-') {
        char c = argv[i][1];
        switch (c) {
            case 'e':
                mode = M_ENC;
                break;
            case 'd':
                mode = M_DEC;
                break;
        default:
            printf("ERROR: unknown option argument: %c\n", c);
            usage(argv[0]);
            return -1;
        }
        i++;
    }
    keyWord = argv[i++];
    if(!keywordValid(keyWord)){
      fprintf(stderr, "The keyword entered was invalid\n");
      return -1;
    }
    ifile = argv[i++];
    ofile = argv[i++];

    // open the files and return -1 if the files can't be opened
    if ( (ifp = fopen(ifile, "r")) == NULL ) {
        fprintf(stderr, "ERROR: Cannot open %s\n", ifile);
        return -1;
    }
    if (( ofp = fopen(ofile, "w")) == NULL ) {
        fprintf(stderr, "ERROR: Cannot open %s\n", ofile);
        return -1;
    }

    /* print command line parameters */
    printf("Mode is %s\n", (mode==M_ENC) ? "encode" : "decode");
    printf("Keyword is %s\n", keyWord);
    printf("Input file is %s\n", ifile);
    printf("Output file is %s\n", ofile);

    /* process all lines of text in input file */
    /* get one line from innput file */
    while (fgets(istr, LINE_LEN, ifp) != NULL) {

        //printf("%s",istr);
        /* encode or decode */
        enc_dec(keyWord, istr, ostr, mode);
        /* print line to output file */
        fprintf(ofp, "%s", ostr);
        fprintf(stderr, "%s", ostr);
    }

    fclose(ifp);
    fclose(ofp);
    return 0;
}

bool keywordValid(char *input)
{
    int len = strlen(input);
    int i;

    if (len == 0) {
        return false;
    }
    for (i = 0; i < len; i++) {
        if ( !isalpha(input[i])) {
            return false;
        }
    }
    return true;
}

void enc_dec(char *key, char *string, char *encodedString, int mode)
{

    int keylen = strlen(key);
    int stringlen = strlen(string);
    char current_char;
    bool upperCase = false;
    int key_index;
    int j = 0;
    int coded_offset;
    char coded_char;

    // encode
    if (mode == M_ENC) {
      // iterate over the entire string
      for(int i = 0; i < (stringlen); i++){
        // if the character need not be encoded
        if(!isalpha(string[i])){
          encodedString[i] = string[i];
        }
        // else if it needs to be changed
        else{
          // get the correct key word character
          key_index = j % keylen;
          //printf("%c", key[key_index]);


          if(isupper(string[i])){
            string[i] = tolower(string[i]);
            upperCase = true;

          }
          // to switch the character, add the keyword - "a" to the current
          coded_offset  = string[i] + (tolower(key[key_index]) - 'a');
          if(coded_offset > 122){
            coded_offset -= 26;
          }
          //if(coded_offset < )
          //printf("%d\n", coded_offset);

          coded_char = coded_offset;
          string[i] = coded_char;

          if(upperCase){
            string[i] = toupper(string[i]);
          }
          encodedString[i] = string[i];

          j++;
        }
        upperCase = false;

      }// end of for loop
      encodedString[i] = 0;

    }// end of encode
    // decode
    else {
        //add code here
        for(int i = 0; i < (stringlen); i++){
          // if the character need not be encoded
          if(!isalpha(string[i])){
            encodedString[i] = string[i];
          }
          // else if it needs to be changed
          else{
            // get the correct key word character
            key_index = j % keylen;
            //printf("%c", key[key_index]);


            if(isupper(string[i])){
              string[i] = tolower(string[i]);
              upperCase = true;

            }
            // to switch the character, add the keyword - "a" to the current
            coded_offset  = string[i] - (tolower(key[key_index]) - 'a');
            if(coded_offset < 97){
              coded_offset += 26;
            }
            //if(coded_offset < )
            //printf("%d\n", coded_offset);

            coded_char = coded_offset;
            string[i] = coded_char;

            if(upperCase){
              string[i] = toupper(string[i]);
            }
            encodedString[i] = string[i];

            j++;
          }
          upperCase = false;

        }// end of for loop

    }
}
