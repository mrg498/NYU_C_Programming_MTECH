// Miles Grossenbacher

#include<stdio.h>
#include <stdbool.h>

int main()
{
  printf("%zu\n", sizeof(char));
  printf("%zu\n", sizeof(int));
  printf("%zu\n", sizeof(short));
  printf("%zu\n", sizeof(long));
  printf("%zu\n", sizeof(long long));
  printf("%zu\n", sizeof(bool));
  printf("%zu\n", sizeof(unsigned int));
  printf("%zu\n", sizeof(float));
  printf("%zu\n", sizeof(double));
  printf("%zu\n", sizeof(long double));
  printf("%zu\n", sizeof(12345));
  printf("%zu\n", sizeof(123456789012));
  printf("%zu\n", sizeof(3.14));
  printf("%zu\n", sizeof(3.14f));
  printf("%zu\n", sizeof("hello"));

  return 0;
}

//a. 12345 is an int and 123456789012 is a long
//b. 3.14 is implicitly treated as a double and 3.14f is explicitly defined as a float
//c. there is a terminating character at the end of strings in c "\o"
//d. The smallest unit for a type allowed in c is 1 byte, so bool also uses 1 byte
