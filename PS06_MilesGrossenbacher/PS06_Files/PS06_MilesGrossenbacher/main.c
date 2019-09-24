#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "main.h"
#include "search.h"

/* useful to get program running, but may want to set to 0 after bugs are out */
#define db_flag     0

/* function prototypes */
int comp_last(const void * a, const void * b);

enum {
    MD_LINEAR = 1,
    MD_BINARY,
    MD_HASH
};

enum {
    LN_FIELD = 0,
    FN_FIELD,
    PN_FIELD,
    NUM_FIELDS
};

int main(int argc, char *argv[])
{
    int mode, i, j, k, n, num_entries, status;
    float avg_chain_len;
    char line[LINE_LEN], *p, *s;
    char* ifile;
    FILE* ifp;
    struct PhoneBook phone_book[PB_LEN];

    /* parse command line
     *  Usage printout
     *  Determine mode
     *  Open phone book file
     */
    //YOUR CODE HERE
    if(argc != 3){
      printf("Usage: search linear|binary|hash phone_book_file.csv\n");
      return -1;
    }


    ifile = argv[2];
    char* choice = argv[1];
    // mode 1=linear, 2=Binary, 3=hash
    if(choice[0] == 'l'){
      mode = MD_LINEAR;
      //printf("L\n" );
    }
    else if(choice[0] == 'b'){
      mode = MD_BINARY;
      //printf("B\n");

    }
    else if(choice[0] == 'h'){
      mode = MD_HASH;
      //printf("H\n" );
    }
    else{
      printf("Error: mode argument incompatible\n");
      printf("Usage: search linear|binary|hash phone_book_file.csv\n");
      return -1;
    }
    //printf("argv1 = %s\n", argv[1]);
    //printf("%d\n", mode);

    if ( (ifp = fopen(ifile, "r")) == NULL ) {
        fprintf(stderr, "ERROR: Cannot open %s\n", ifile);
        return -1;
    }

    /* initialize phone book structure */
    //YOUR CODE HERE
    for(i = 0; i<PB_LEN; i++){
      phone_book[i].first[0] = 0;
      phone_book[i].last[0] = 0;
      phone_book[i].phone[0] = 0;
      phone_book[i].chain = -1;
    }

    /* read in phone book
     * use fgets() and strtok()
     */

    char* token;
    int token_length = 0;
    n=0; /* counter for lines read and entries in phone book */
    while (fgets(line, LINE_LEN, ifp) != NULL) {
        int token_num = 0;
        if (db_flag) printf("%d %s", n, line);
        /* tokenize each field using comma as field separator */
        //YOUR CODE HERE
        token = strtok(line, ",\n");

      	strcpy(phone_book[n].last, token);
        token_length = strlen(token);
        phone_book[n].last[token_length] = 0;
      	token_num++;

      	while ( (token = strtok(NULL, ",\n") ) != NULL) {
      		 	if(token_num == 1){
      				strcpy(phone_book[n].first,token);
              token_length = strlen(token);
              phone_book[n].first[token_length] = 0;
      			}

      			else{
      				strcpy(phone_book[n].phone,token);
              token_length = strlen(token);
              phone_book[n].phone[token_length] = 0;
      			}
      			token_num++;
      	 }

        n++;
        if (n >= PB_LEN)
            break;
    }
    fclose(ifp);
    num_entries = n;
    printf("%d Entries in phone book\n", num_entries);

    /* processing prior to search */
    switch (mode) {
        case MD_BINARY:
            /* binary search assumes list is sorted in alpha-numeric order
             * so sort phone book by last name
             */
            qsort(phone_book, num_entries, sizeof(phone_book[0]), comp_last );
            break;
        case MD_HASH:
            create_hash_map(phone_book, num_entries);
            break;
    }

    /* print phone book */
    if (db_flag) for (i=0; i<num_entries; i++) {
        printf("%s, %s - %s\n",
            phone_book[i].last,
            phone_book[i].first,
            phone_book[i].phone);
    }
    printf("\n");

    while (1) {
        /* now ask for any last name */
        printf("Input any last name in phone book (0 to exit):\n");
        scanf("%s", line);
        if (line[0] == '0') {
            break; /* quit */
        }
        if (db_flag) printf("You entered:  %s\n", line);

        switch (mode) {
            case MD_LINEAR:
                /* linear search */
                i = linear_search(line, phone_book, num_entries);
                break;
            case MD_BINARY:
                /* binary search */
                i = binary_search(line, phone_book, num_entries);
                break;
            case MD_HASH:
                /* hashed search */
                i = hash_search(line, phone_book);
                break;
        }
        if (i < 0) {
            fprintf(stderr, " ERROR: %s not found\n", line);
        }
        else {
            printf("%s, %s - %s\n",
                phone_book[i].last,
                phone_book[i].first,
                phone_book[i].phone);
        }
    }
    return 0;
}

/* user-suppied ordering function for qsort() */
int comp_last(const void * a, const void * b)
{
    int sortVal;
	struct PhoneBook *p1 = (struct PhoneBook *)a;
	struct PhoneBook *p2 = (struct PhoneBook *)b;
	return strncmp(p1->last, p2->last, NAME_LEN-1);
};
