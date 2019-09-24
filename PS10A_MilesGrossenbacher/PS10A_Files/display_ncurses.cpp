#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>     /* memset */
#include <ctype.h>      /* loupper() */
#include <math.h>       /* round */
#include <sndfile.h>    /* libsndfile, for my debugging output file */
#include <ncurses.h>    /* This library is for getting input without hitting return */
#include "room_acoustics.h"

/* function prototypes for functions in this file */
void print_room(Room *room);

void display_ncurses( void )
{
    int ch;
    extern Room *room;
    extern int samplerate;

    /* Initialize ncurses
     * to permit interactive character input
     */
    initscr(); /* Start curses mode */
    keypad(stdscr, true); /* For arrow keys */
    cbreak();  /* Line buffering disabled */
    noecho(); /* Uncomment this if you don't want to echo characters when typing */

    ch = '\0'; /* Init ch to null character */
    Pt *ps = &room->source;
    Pt *pl = &room->listener;
    while (toupper(ch) != 'Q') {
        //for debug output file
        extern SNDFILE *osfile;
        extern float *osignal;
        extern int ocount;
        clear();
        //printw("%c\n", ch);
        /* print configuration */
        printw("Source Position: %6.2f %6.2f %6.2f\n", ps->x, ps->y, ps->z);
        printw("Use arrow keys to move listener\n");
        printw("Listener Position: %6.2f %6.2f %6.2f (%6.2f)\n",
            pl->x, pl->y, pl->z, get_dist(ps, pl));
        printw("Use +/- keys to make room larger or smaller\n");
        printw("Room dimensions: %6.2f %6.2f %6.2f\n",
            room->dx*2, room->dy*2, room->dz*2);
        printw("Use R/r keys to make wall reflection larger or smaller (%4.2f)\n",
            room->wall_reflection);
        printw("Use I/i keys to enable/disable phantom images (%s)\n",
            room->images ? "on " : "off");
#ifdef  PARA_REVERB
        printw("Use P/p keys to enable/disable parametric reverberation (%s)\n",
            room->parametric ? "on " : "off");
#endif
        printw("Q to quit\n");
        printw("\n");
        calc_images(room);
        print_room(room);
        refresh();

        ch = getch();
        Pt t;
        /* temp values */
        t.x = pl->x;
        t.y = pl->y;
        t.z = pl->z;
        /* integer room dimensions */
        int idx = round(room->dx);
        int idy = round(room->dy);
        switch(ch) {
            /* move listener
             * but check that move is not closer than 1 m from room wall
             * or closer than 2 m from source
             */
            case KEY_LEFT:
                t.x -= 1;
                if ( t.x > -idx && get_dist(&t, ps) > MIN_DIST) {
                    pl->x = t.x;
                }
                break;
            case KEY_UP:
                t.y -= 1;
                if ( t.y > -idy && get_dist(&t, ps) > MIN_DIST) {
                    pl->y = t.y;
                }
                break;
            case KEY_RIGHT:
                 t.x += 1;
                if ( t.x < idx && get_dist(&t, ps) > MIN_DIST) {
                    pl->x = t.x;
                }
                break;
            case KEY_DOWN:
                t.y += 1;
                if ( t.y < idy && get_dist(&t, ps) > MIN_DIST) {
                    pl->y = t.y;
                }
                break;
            case '+':
            case '-':
                if (ch == '+') {
                    room->dx *= 1.1;
                    room->dy *= 1.1;
                    room->dz *= 1.1;
                }
                else {
                    room->dx /= 1.1;
                    room->dy /= 1.1;
                    room->dz /= 1.1;
                }
                /* re-calculate source and listener positions */
                    room->source.x = -5;
                    room->source.y = 0;
                    room->source.z = 0;
                    room->listener.x = +5;
                    room->listener.y = 0;
                    room->listener.z = 0;
                break;
            case 'R':
                room->wall_reflection *= R_FAC;
                if (room->wall_reflection > 1.0)
                    room->wall_reflection = 1.0; //cannot be greater than 1.0
                break;
            case 'r':
                room->wall_reflection /= R_FAC;
                break;
            case 'I':
                room->images = true;
                break;
            case 'i':
                room->images = false;;
                break;
            case 'P':
                room->parametric = true;
                break;
            case 'p':
                room->parametric = false;;
                break;
            case 'D':
                room->delay_info = true;
                break;
            case 'd':
                room->delay_info = false;;
                break;
            case 'Q':
            case 'q':
                /* close debug output WAV file */
                if (osfile) {
                    sf_writef_float(osfile, osignal, ocount);
                    printf("Closing output WAV file\n");
                    sf_close(osfile);
                }
                break;
        }
    }
    /* End curses mode  */
    endwin();
}


void print_room(Room *room)
{
    int dxi = round(room->dx);
    int dyi = round(room->dy);
    int sx = round(room->source.x);
    int sy = round(room->source.y);
    int lx = round(room->listener.x);
    int ly = round(room->listener.y);

    for (int y=-dyi; y<=dyi; y++) { //rows
        for (int x=-dxi; x<=dxi; x++) { //columns
            //YOUR CODE HERE
            if((x == -dxi && y == -dyi) || (x == -dxi && y == dyi)){
              printw("+");
            }
            else if((x == dxi && y == -dyi) || (x == dxi && y == dyi)){
              printw("+\n");
            }
            else if(x == -dxi){
              printw("|");
            }
            else if(x == dxi){
              printw("|\n");
            }
            else if(y == -dyi){
              printw("-");
            }
            else if(y == dyi){
              printw("-");
            }
            else if(x == sx && y == sy){
              printw("X");
            }
            else if(x == lx && y == ly){
              printw("O");
            }
            else{
              printw(" ");
            }
        }
    }
}
