// Miles Grossenbacher
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main()
{
  char line[512] = {0,};
  while (true) {
    printf("Enter a line of text:\n");
    fgets(line, sizeof(line), stdin);
    if (line[0] == '\n') {
      puts("bye");
      break;
    }
    // each token will hold one substring of continuos characters until a space
    char *token;
    token = strtok(line, " ");

    printf("\n");
    printf("%s\n", "you typed:");

    // while loop goes through string and delimits at spaces,
    while(token != NULL){
      printf("%s\n", token);
      token = strtok(NULL, " ");
    }
  }

  return 0;
}
