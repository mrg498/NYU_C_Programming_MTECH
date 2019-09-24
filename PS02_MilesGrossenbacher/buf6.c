// Miles Grossenbacher

#define _FORTIFY_SOURCE 0 // turns off some warnings
#include <stdio.h>
#include <string.h>
int main(void) {
 char src[24] = "C programming";
 char dest[16];
 scanf("%15s", dest);
 printf("dest: %s\n", dest);
 printf("src : %s\n", src);
 return 0;
}
