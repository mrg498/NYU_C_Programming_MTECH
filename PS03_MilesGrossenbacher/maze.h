/* constants */
#define DIM_I 25    //Rows
#define DIM_J 70    //Column
#define START_MARKER 'S'
#define END_MARKER 'G'
#define VISITED_MARKER '.'
#define SOLUTION_MARKER '*'
#define WALL_MARKER '|'
#define FRAME_RATE_DEFAULT 200000

extern char grid[DIM_I][DIM_J];

/* function prototype */
bool solveMaze(int i, int j);
void display();
