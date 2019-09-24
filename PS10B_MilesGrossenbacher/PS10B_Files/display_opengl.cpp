#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>	/* libsndfile, for my debugging output file */
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "room_acoustics.h"

/* initial view rotation angles */
static float ang_x = 15;
static float ang_y = -35;
static float ang_z = 0;

/* state variable for moving source or listener */
#define MOVE_SOURCE		0
#define MOVE_LISTENER	1
static int sl = 0; //source if 1, listener if 0

/* room data structure
 * extern global for openGL rendering
 */
 extern Room *room;

// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display( void );
void normalKeys( unsigned char key, int x, int y );
void specialKeys( int key, int x, int y );

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
int wh;
int wd = 500;
int ht = 500;
double scale = 1.0;

void display_openGL( void )
{
	// Simulate command line args (which are not used)
	int argc = 0;
	char *argv[1];
	//  Initialize GLUT
	glutInit(&argc, argv);

	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Set the window's initial width & height
	glutInitWindowSize(wd, ht);
	// Position the window's initial top-left corner
	glutInitWindowPosition(50, 50);
	// Create window and store window handle
	wh = glutCreateWindow("Room Reverberation Model");

	//  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	// Set line width
  	glLineWidth(3.0);

	// Callback functions
	glutDisplayFunc(display);
	glutKeyboardFunc(normalKeys);
	glutSpecialFunc(specialKeys);

	//  Pass control to GLUT for events
	glutMainLoop();
}

// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display()
{
	float dx, dy, dz;

    calc_images(room);

	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Rotate for better perspective
	glRotatef( ang_x, 1.0, 0.0, 0.0 );
	glRotatef( ang_y, 0.0, 1.0, 0.0 );
	glRotatef( ang_z, 0.0, 0.0, 1.0 );

	// Scale
	glScalef( scale, scale, scale );

	// Wire cube from built-in (glut library) functions
	glutWireCube(2*room->dx/SCALE); //cube dimension

    // source sphere.
    glPushMatrix();
    glTranslatef(room->source.x/SCALE,
    	room->source.y/SCALE,
    	room->source.z/SCALE);
	glColor3f( 1.0, 0.0, 0.0 );  // Red
    glutSolidSphere(SPHERE_RAD/SCALE,20,20);
    glPopMatrix();

    // listener sphere
    glPushMatrix();
    glTranslatef(room->listener.x/SCALE,
    	room->listener.y/SCALE,
    	room->listener.z/SCALE);
	glColor3f( 1.0, 1.0, 1.0 );  // White
    glutSolidSphere(SPHERE_RAD/SCALE,20,20);
    glPopMatrix();

	// image spheres
	for (int i=1; i<=room->num_images; i++) {
	    glPushMatrix();
	    glTranslatef(room->image[i].loc.x/SCALE,
	    	room->image[i].loc.y/SCALE,
	    	room->image[i].loc.z/SCALE);
		glColor3f( 1.0, 1.0, 0.0 );  // Yellow
	    glutSolidSphere(SPHERE_RAD/SCALE,20,20);
	    glPopMatrix();
	}
	//reset draw color
	glColor3f( 1.0, 1.0, 1.0 );  // White

	glFlush();
	glutSwapBuffers();
}

// ----------------------------------------------------------
// gultKeyboardFunc() Callback Function
// ----------------------------------------------------------
void normalKeys( unsigned char key, int x, int y )
{
	//for debug output file
	extern SNDFILE *osfile;
	extern float *osignal;
	extern int ocount;

  // pointers to source and listener points
  Pt *ps = &room->source;
  Pt *pl = &room->listener;

  // extracted the room dimensions
  int idx = round(room->dx);
  int idy = round(room->dy);
  int idz = round(room->dz);

	//temporary source and listener positions
	Pt s, l;
	s.x = room->source.x;
	s.y = room->source.y;
	s.z = room->source.z;
	l.x = room->listener.x;
	l.y = room->listener.y;
	l.z = room->listener.z;
	switch (key) {
		/* Set up for moving source or listener
		 * and then move source or listener
		 */
		//YOUR CODE HERE

    // move source activated
    case 'S':
    case 's':
        sl = MOVE_SOURCE;
        break;

    // move listener activated
    case 'L':
    case 'l':
        sl = MOVE_LISTENER;
        break;

    // move source or listener positive x
    case 'X':
        if(sl == MOVE_LISTENER){
          l.x += 1;
          if ( l.x < idx && get_dist(&l, ps) > MIN_DIST) {
              pl->x = l.x;
          }
        }

        else{
          s.x += 1;
          if ( s.x < idx && get_dist(&s, pl) > MIN_DIST) {
              ps->x = s.x;
          }

        }
        break;

    // move source or listener negative x
    case 'x':
        if(sl == MOVE_LISTENER){
          l.x -= 1;
          if ( l.x > -idx && get_dist(&l, ps) > MIN_DIST) {
              pl->x = l.x;
          }
        }
        else{
          s.x -= 1;
          if ( s.x > -idx && get_dist(&s, pl) > MIN_DIST) {
              ps->x = s.x;
          }

        }
        break;

    case 'Y':
        if(sl == MOVE_LISTENER){
          l.y += 1;
          if ( l.y < idy && get_dist(&l, ps) > MIN_DIST) {
              pl->y = l.y;
          }
        }
        else{
          s.y += 1;
          if ( s.y < idy && get_dist(&s, pl) > MIN_DIST) {
              ps->y = s.y;
          }
        }
        break;

    case 'y':
        if(sl == MOVE_LISTENER){
          l.y -= 1;
          if ( l.y > -idy && get_dist(&l, ps) > MIN_DIST) {
              pl->y = l.y;
          }
        }
        else{
          s.y-= 1;
          if ( s.y > -idy && get_dist(&s, pl) > MIN_DIST) {
              ps->y = s.y;
          }

        }
        break;

    case 'Z':
        if(sl == MOVE_LISTENER){
          l.z += 1;
          if ( l.z < idz && get_dist(&l, ps) > MIN_DIST) {
              pl->z = l.z;
          }
        }
        else{
          s.z += 1;
          if ( s.z < idz && get_dist(&s, pl) > MIN_DIST) {
              ps->z = s.z;
          }
        }
        break;

    case 'z':
        if(sl == MOVE_LISTENER){
          l.z -= 1;
          if ( l.z > -idz && get_dist(&l, ps) > MIN_DIST) {
              pl->z = l.z;
          }
        }
        else{
          s.z -= 1;
          if ( s.z > -idz && get_dist(&s, pl) > MIN_DIST) {
              ps->z = s.z;
          }

        }
        break;

		// Make cube larger or smaller
		case '+':
			scale *= 1.1;
			break;
		case '-':
			scale /= 1.1;
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

 		// Quit if q or Q or ESC
 		case 'q':
 		case 'Q':
 		case 27:
		    /* close debug output WAV file */
		    if (osfile) {
		    	sf_writef_float(osfile, osignal, ocount);
		    	printf("Closing output WAV file\n");
		        sf_close(osfile);
		    }
		    /* close glut window and exit */
	 		glutDestroyWindow(wh);
	 		exit(0);
	 		break;
 	}

	//  Request display update
	glutPostRedisplay();
	//END OF YOUR CODE
}

// ----------------------------------------------------------
// glutSpecialFunc() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y )
{
	// Rotate scene
	//YOUR CODE HERE
  switch(key){
    case GLUT_KEY_RIGHT:
        ang_y--;
        break;

    case GLUT_KEY_LEFT:
        ang_y++;
        break;

    case GLUT_KEY_UP:
        ang_x++;
        break;

    case GLUT_KEY_DOWN:
        ang_x--;
        break;
  }

  glutPostRedisplay();
}
