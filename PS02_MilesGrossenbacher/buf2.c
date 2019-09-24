// Miles Grossenbacher

#define _FORTIFY_SOURCE 0 // turns off some warnings
#include <stdio.h>
#include <string.h>
int main(void) {
 char src[24] = "C programming is so fun";
 char dest[16];
 strcpy(dest, src);
 printf("dest: %s\n", dest);
 printf("src : %s\n", src);
 return 0;
}
