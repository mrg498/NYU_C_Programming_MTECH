// Miles Grossenbacher
#include <stdbool.h>
#include "maze.h"

bool solveMaze(int i, int j)
{
  if(grid[i][j] == 'G'){
    return true;
  }
  else if((grid[i][j] == '|') || (grid[i][j] == '.')){
    return false;
  }
  else if((i < 0 || i >= DIM_I) || (j < 0 || j >= DIM_J)){
    return false;
  }
  else{
    grid[i][j] = '.';
    display();

    // member variables to define directions
    int Ni = i-1;
    int Si = i+1;
    int Ei = i;
    int Wi = i;

    int Nj = j;
    int Sj = j;
    int Ej = j-1;
    int Wj = j+1;

    if(solveMaze(Ni,Nj) || solveMaze(Si,Sj) || solveMaze(Ei,Ej) || solveMaze(Wi,Wj)){
      grid[i][j] = SOLUTION_MARKER;
      display();
      return true;
    }
    else{
      return false;
    }


  }

}
