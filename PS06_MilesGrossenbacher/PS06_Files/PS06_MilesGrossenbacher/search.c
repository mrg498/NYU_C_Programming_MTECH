#include <stdio.h>
#include <string.h>
#include "main.h"

#define HV_BITS     14      /* bit length of hash */
#define HT_LEN      (1<<HV_BITS)    /* hash table length */

#define db_flag 1

static int hash_map[HT_LEN];

/*
 * linear search
 */
int linear_search(char *target, struct PhoneBook *phone_book, int num_entries)
{
    //YOUR CODE HERE
    //int target_size = strlen(target);

    for(int i = 0; i<num_entries; i++){
      //printf("%s\n", target);

      char* last_name = phone_book[i].last;
      //printf("%s\n", last_name);
      if(strcmp(last_name, target) == 0){
        printf("%s\n", "found it!");
        return i;
      }
    }
    //printf("didn't find %s\n", target);
    return -1;

}

/*
 * binary search
 */
int binary_search(char *target, struct PhoneBook *phone_book, int num_entries)
{
     /* binary search
     * low is low index in search interval
     * high is high index in search interval
     */
    //char* last_name = phone_book[i].last;
    int low, mid, high, status, n, i;
    low = 0;
    high = num_entries;
    mid = (low+high)/2;
    n = 0;
    while (low <= high) {
    //YOUR CODE HERE
      n++;
      if(strcmp(target, phone_book[mid].last) == 0){
        i = mid;
        break;
      }
      else if(strcmp(target, phone_book[mid].last) < 0){
        printf("Iteration\n" );
        high = mid;
        mid = (low+high)/2;
      }
      else{
        printf("Iteration\n" );
        low = mid;
        mid = (low+high)/2;
      }
      // if mid has no number inbetween low and high,
      // because low and high are only 1 number apart
      //increment low by two to get it above high to break cycle
      if(low == mid){
        low += 2;
      }
        if (db_flag) printf("%d %d %d %d %d %s %s\n",
            n, low, mid, high, status, phone_book[mid].last, target);
    }
    if (low > high) {
        //fprintf(stderr, "ERROR: %s not found\n", target);
        return -1;
    }
    return i;
}

/*
 * hash map search
 */
/* hash function */
int hash_func(char *s)
{
    int i;
    int h = 0, g;
    for (i=0; s[i]; i++) {
        #if 0
        h += s[i]; /* poor hash fuction -- hash has max value of 1041 */
        #else
        h = s[i] + (h << 6) + (h << 16) - h; /* h = s[i] + h*65631 */
        #endif
    }
    h &= (1<<HV_BITS)-1; /* mask to length of hash value */
    return h;
}

/* create hash map */
void create_hash_map(struct PhoneBook *phone_book, int num_entries)
{
    int i, j, k, n, status, num_collisions, chain_len;
    float avg_chain_len;;

    /* initialize hash map table */
    for (i=0; i<HT_LEN; i++) {
        hash_map[i] = -1; /* an invalid entry */
    }

    /* create hash for each last name in book */
    for (i=0; i<num_entries; i++) {
        //YOUR CODE HERE
        j  = hash_func(phone_book[i].last);
        if(hash_map[j] == -1){
          hash_map[j] = i;
        }
        else{
          k = hash_map[j];
          while(phone_book[k].chain >= 0){
            k = phone_book[k].chain;
          }
          if (phone_book[k].chain == -1){
            phone_book[k].chain = i;
          }
        }


        /* debug printout, where j is hash value */
        if (db_flag) printf("%6d %6d %s\n", i, j, phone_book[i].last);
    }

    /* report statistics of hash table */
    k=0;
    n=0;
    num_collisions = 0;
    avg_chain_len = 0;
    for (i=0; i<HT_LEN; i++) {
        if (hash_map[i] >= 0) {
            /* hash map occupied */
            k = i;
            n++;
            j = hash_map[i];
            if (phone_book[j].chain >= 0) {
                /* collision for this hash value */
                num_collisions++;
                chain_len = 1;
                j = phone_book[j].chain;
                while (phone_book[j].chain >= 0) {
                    chain_len++;
                    j = phone_book[j].chain;
                }
                if (db_flag) printf("Entry %d has chain length %d\n", i, chain_len);
                avg_chain_len += chain_len;
            }
        }
    }
    avg_chain_len /= num_collisions;
    printf("Hash table statistics:\n");
    printf("%6.2f%% map occupancy; top map entry %d out of %d entries\n",
        100.0*n/HT_LEN, k, HT_LEN);
    printf("Collisions: %d (%6.2f%%)\n", num_collisions, 100.0*num_collisions/HT_LEN);
    printf("Average chain length %6.2f\n", avg_chain_len);
}

/* hash search */
int hash_search(char *target, struct PhoneBook *phone_book)
{

    //YOUR CODE HERE
    int target_hash;
    int i;
    target_hash = hash_func(target);
    i = hash_map[target_hash];
    while(strcmp(phone_book[i].last, target) != 0){
      if(phone_book[i].chain == -1){
        return -1;
      }
      i = phone_book[i].chain;
    }
    return i;
}
