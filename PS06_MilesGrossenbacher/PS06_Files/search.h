#ifndef _SEARCH_H
#define _SEARCH_H

int linear_search(char *target, struct PhoneBook *phone_book, int num_entries);
int binary_search(char *target, struct PhoneBook *phone_book, int num_entries);
void create_hash_map(struct PhoneBook *phone_book, int num_entries);
int hash_search(char *target, struct PhoneBook *phone_book);

#endif
