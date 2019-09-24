// Miles Grossenbacher

#include<stdio.h>

int main()
{
  printf("%.20f",(float)1.0);
  printf("%.20f",(float)0.5);
  printf("%.20f",(float)0.1);

  return 0;
}

/* the 0.1 is incorrect because of how decimal numbers are represented by the machine's hardware.
If the denominater of the humber is not a power of two then we will get rounding errors because
our machine has to try and represent any number in base 2 binary. The other number do have
base 2 denominaters so they can be represented without rounding*/
