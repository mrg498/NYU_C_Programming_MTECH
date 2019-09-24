// Miles Grossenbacher
#include <stdio.h>

int main()
{
  // 16x16 matrix
  char pascal[32][32];
  // outer loop iterates over 15 rows
  for(int i = 0; i < 32; i++){
    // inner loop iterates over columns with length == row number
    for(int j = 0; j <= i; j++){
      // if the number is the first or last in a collumn
      // it is automatically 1
      if(j == 0 || j == i){
        pascal[i][j] = 1;
      }
      // for every other number, the value will be the previous row at spot j
      // and j-1 added together
      else{
          pascal[i][j] = (pascal[i-1][j-1]) + (pascal[i-1][j]);
      }
      // if it is not the last number in the row, the next number
      // will be on the same line
      if(j<i){
        if((pascal[i][j] % 2) != 0){
          printf("%5s", "*");
        }
        else{
          printf("%5s"," ");
        }
      }
      // if it is the last number in the row, it will be a star and go to a new line
      else{
        printf("%5s\n", "*");
      }
    }
  }

  return 0;
}
