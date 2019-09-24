// Miles Grossenbacher

#include <stdio.h>

int main(void)
{
printf("%s\n", 2 ? "true": "false" );
printf("%s\n", 3 ? "true": "false" );
printf("%s\n", 3.14 ? "true": "false" );
printf("%s\n", 0.0 ? "true": "false" );
printf("%s\n", 0.0f ? "true": "false" );
printf("%s\n", "hello" ? "true": "false" );

return 0;
}


/* if the statement "if" evaluates as 0, then it will be false. Any non-zero evaluation
will be treated as true */
