// Miles Grossenbacher

#include<stdio.h>

int main()
{
  printf("%.20f",(double)1.0);
  printf("%.20f",(double)0.5);
  printf("%.20f",(double)0.1);

  return 0;
}

/* In this case, 0.1 is still inaccurate for the same reason, but it is closer to 0.1 because a double type
has double the number of binary places to represent the number, which means it need not rounded as much.*/
