#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>     /* strtok */
#include <math.h>       /* sqrt */
#include <ncurses.h>
#include "room_acoustics.h"

double get_dist(Pt *src, Pt *lis)
{
    //YOUR CODE HERE
    double dist = 0.0;
    double delta_x = (src->x) - (lis->x);
    double delta_y = (src->y) - (lis->y);
    double delta_z = (src->z) - (lis->z);

    dist = sqrt((delta_x*delta_x) + (delta_y*delta_y) + (delta_z*delta_z));

    return dist;
}

double get_dist_atten(double d)
{
    /* normalize full volume to distance of MIN_DIST m
     * intensity decreases at 1/(distance^2)
     * level decreases at 1/distance
     */
    return MIN_DIST/d;
}

void calc_images(Room *room)
{
    Image *p;

    /* default for maximum image delay */
    room->max_delay = 0;

    /* direct path (images[0]) */
    p = &room->image[0];
    p->distance = get_dist(&room->source, &room->listener);
    p->samp_delay = round(room->samplerate * (p->distance/SpeedOfSound));
    p->atten = get_dist_atten(p->distance);;
    if (room->delay_info) {
        PRINT("Path %d: delay %d, atten %f\n", 0, p->samp_delay, p->atten);
    }

    /* calculate position of phantom sound images */
    for (int i=1; i<=room->num_images; i++) {
        int d;
        /* point to image */
        p = &room->image[i];
        /* default */
        p->loc.x = room->source.x;
        p->loc.y = room->source.y;
        p->loc.z = room->source.z;
        switch (i) {
            /* construct 6 images for the 6 surfaces of room */
            //YOUR CODE HERE -- add remaining cases

            //right
            case 1:
                d = abs(room->dx - room->source.x);
                p->loc.x = room->source.x + 2*d;
                break;

            case 2:
                d = abs(room->dy - room->source.y);
                p->loc.y = room->source.y + 2*d;
                break;
            case 3:
                d = abs(room->dz - room->source.z);
                p->loc.z = room->source.z + 2*d;
                break;

            //  walls
            case 4:
                d = abs(-room->dx + room->source.x);
                p->loc.x = room->source.x - 2*d;
                break;
            case 5:
                d = abs(-room->dy + room->source.y);
                p->loc.y = room->source.y - 2*d;
                break;
            case 6:
                d = abs(-room->dz + room->source.z);
                p->loc.z = room->source.z - 2*d;
                break;


        }
        /* Calculate the image distance, in meters (default unit)
         * and from this, the sample delay
         * finally, calculate the attenuation due to wall reflection and
         * distance attenuation
         */
        //YOUR CODE HERE

        p->distance = get_dist(&p->loc, &room->listener);
        p->samp_delay = round(room->samplerate * (p->distance/SpeedOfSound));
        p->atten = get_dist_atten(p->distance);;

        /* calculate max delay */
        if (p->samp_delay > room->max_delay) {
            room->max_delay = p->samp_delay;
        }

        /* print information about image path */
        if (room->delay_info) {
            PRINT("Path %d: delay %d, atten %f\n", i, p->samp_delay, p->atten);
        }
    }
}
