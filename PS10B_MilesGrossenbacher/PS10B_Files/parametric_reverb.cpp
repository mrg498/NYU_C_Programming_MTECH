#include "para_reverb.h"

/* allpass */
#define	G 	0.7
#define M1	1051
#define M2	337
#define M3	113
/* comb */
#define G4	0.742
#define G5	0.733
#define G6	0.715
#define G7	0.697
#define M4	4799
#define	M5	4999
#define M6	5399
#define M7	5801

/* global objects */

/* allpass filters */
paraReverb apf1(ALLPASS, G, G, M1);
paraReverb apf2(ALLPASS, G, G, M2);
paraReverb apf3(ALLPASS, G, G, M3);
/* comb filters */
paraReverb cf1(COMB, G4, 1, M4);
paraReverb cf2(COMB, G5, 1, M5);
paraReverb cf3(COMB, G6, 1, M6);
paraReverb cf4(COMB, G7, 1, M7);

float parametric_reverb(float x0, float reflect)
{
	float x1, x2, x3, y;
 	x1 = apf1.filter(x0);
	x2 = apf2.filter(x1);
	x3 = apf3.filter(x2);

	y  = cf1.filter(x3);
	y += cf2.filter(x3);
	y += cf3.filter(x3);
	y += cf4.filter(x3);

	y /= (4 * 12); //second factor is to normalize parametric and image signal power
	y *= reflect;
	return y;
}

void clear_reverb(void)
{
 	apf1.clear();
	apf2.clear();
	apf3.clear();

	cf1.clear();
	cf2.clear();
	cf3.clear();
	cf4.clear();
}