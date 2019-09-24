#ifndef  _MAIN_H
#define  _MAIN_H

#define LINE_LEN    80      /* length of line in CSV phone book file */
#define NAME_LEN    64      /* name string length */
#define NUM_LEN     11      /* 10 digit US phone numbers plus NULL*/
#define PB_LEN      1024    /* max phone book length */


struct PhoneBook {
    char first[NAME_LEN];
    char last [NAME_LEN];
    char phone[NUM_LEN];
    int chain;
};

#endif
