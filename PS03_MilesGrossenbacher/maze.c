/***************************************************************************
 * maze.c
 *
 * Solve a maze using recursive backtracking
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "maze.h"

/* global: array containing maze */
char grid[DIM_I][DIM_J];

/* file where maze environment is found */
const char *mazeFilename = "maze.txt";

/* frame rate */
float frameRateSetting = .1;

// function prototypes
void clear();
bool solveMaze(int i, int j);
void drawGrid();
void readFile(const char *fileName);
void setStart(int *i, int *j);

int main(int argc, char *argv[])
{
    char ch;

    /* current position in maze */
    int pos_i, pos_j;

    /* check to make sure there are either 0 or 1 arguments */
    if (argc > 2) {
        printf("Usage: %s [rate]\n", argv[0]);
        return 1;
    }

    /* if an argument is supplied... */
    if (argc == 2) {
        frameRateSetting = atof(argv[1]);
    }

    // initialize the maze
    readFile(mazeFilename);

    clear();
    drawGrid();

    // find start point
    setStart(&pos_i, &pos_j);
    printf("Press return to start maze solver ");
    scanf("%c", &ch);

    // Try to solve the maze
    if (solveMaze(pos_i, pos_j)) {
        printf("Maze solved!\n");
    } else {
        printf("Maze is unsolvable.\n");
    }

    return 0;
}

void display() {
    clear();
    drawGrid();
    usleep(frameRateSetting * FRAME_RATE_DEFAULT);
}

void setStart(int *pi, int *pj)
{
    int i, j;

    for (i = 0; i < DIM_I; i++) {
        for (j = 0; j < DIM_J; j++) {
            if (grid[i][j] == START_MARKER) {
                *pi = i; *pj = j;
                return;
            }
        }
    }
    /* failure to find starting position */
    *pi = -1; *pj = -1;
    return;
}

/* Clears screen using ANSI escape sequences. */
void clear()
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/* Prints the grid in its current state. */
void drawGrid() {

    int i, j;

    for (i = 0; i < DIM_I; i++) {
        for (j = 0; j < DIM_J; j++) {
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
}

/* Read in a file with an preset environment */
void readFile(const char *filename)
{
    FILE *fp;
    char ch;
    int i, j;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }

    for (i = 0; i < DIM_I; i++) {
        for (j = 0; j < DIM_J; j++) {
            ch = fgetc(fp);
            if (ch==EOF) {
                printf("BAD FILE.\n");
                if (ch==EOF) {
                    printf("EOF reached\n");
                }
                return;
            }
            grid[i][j] = ch;
        }
        ch = fgetc(fp); // for return character
    }
    fclose(fp);
}
