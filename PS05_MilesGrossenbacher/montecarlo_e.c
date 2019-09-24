#include <stdio.h>
#include <stdlib.h>

/* the apporximation is correct to the 4th decimal place
   so I would say it it quite similar to the actual value of 'e' */

int sumToOne(){
  float sum = 0;
  int counter = 0;
  float random_num;
  while(sum < 1){
    random_num = (rand() / (float)RAND_MAX);
    //printf("%f\n",random_num);
    sum+=random_num;
    counter++;
  }
  return counter;
}


int main(int argc, char* argv[])
{
  int sum_counts = 0;
  float average_count;
  int n = 0;


  printf("How many times would you like to iterate (i.e. 100000): ");
  scanf("%d", &n);


  for(int i = 0; i < n; i++){
    sum_counts += sumToOne();
  }
  average_count = sum_counts/(float)n;
  printf("%0.9f\n", average_count);

  return 0;
}
