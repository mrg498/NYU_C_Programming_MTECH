#ifndef ROOM_H
#define ROOM_H

#define LINE_LEN        80      /* of parameter file */
#define SpeedOfSound    343.0   /* meters/second, speed of sound in air */
#define FRAMES_PER_BUFFER 1024  /* processing block length */
#define XDIM            30      /* initial room dimensions */
#define YDIM            30      /* room is centered at origin */
#define ZDIM            30      
#define MIN_DIST        2.0     /* minimum distance from source to listener */
#define W_REF           0.9     /* default wall reflection */
#define R_FAC           1.189   /* 1.5 dB */
#define MAX_IMAGES      7       /* direct path plus 6 room walls */
#define MAX_REVERB      2       /* maximum reverb ring-out, seconds */
//for openGL:
#define SCALE           (3.0*XDIM/2.0) /* scale to put all objects within view */
#define SPHERE_RAD      1       /* radius of source, listener, image spheres */

/* defines a point in 3 dimensions */
typedef struct {
    double x;
    double y;
    double z;
} Pt;

/* defines a phantom image source */
typedef struct {
    Pt loc; // image location
    double distance; // from image to listener
    unsigned int samp_delay; // delay of image source, in samples
    double atten; // attenuation of image source
} Image;

/* defines the room with acoustic properties, source and listener locations,
 * phantom images and control variables
 */
typedef struct {
    double dx;  //from center to room wall
    double dy;
    double dz;
    double wall_reflection;
    Pt source;
    Pt listener;
    int num_images;
    Image image[MAX_IMAGES];
    int max_delay;
    int samplerate;
    bool delay_info;    //flags
    bool images;
    bool parametric;
} Room;

/* function prototypes */
void calc_images(Room *room);
double get_dist(Pt *src, Pt *lis);

/* printx() and display_xxxx() functions for use with
 * ncurses or openGL
 * PARA_REVERB is defined only in openGL version
 */
#if USE_NCURSES
#define PRINT printw
#define DISPLAY display_ncurses
#undef  PARA_REVERB
#define PARA_STATE  false
#endif
#if USE_OPENGL
#define PRINT printf
#define DISPLAY display_openGL
#define PARA_REVERB
#define PARA_STATE  true
#endif

#endif