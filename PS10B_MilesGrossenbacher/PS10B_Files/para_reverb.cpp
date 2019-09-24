#include <stdlib.h>     /* calloc, exit, free */
#include "para_reverb.h"

//constructor
paraReverb::paraReverb(int mode_in, float a_in, float b_in, int m_in)
{
	mode = mode_in;
	a = a_in;
	b = b_in;
	m = m_in;
	//allocate and zero buffer
	buf = (float *)calloc(m, sizeof(float));
	//these are pointers
	bottom = buf; //bottom storage
	top = buf+m-1; //top storage
	//initial first and last
	first = bottom; //first storage in delay line
	last = top; //last storage in delay line
}

//destructor
paraReverb::~paraReverb()
{
	if (buf)
		free(buf);
}

//filter using circular buffer
float paraReverb::filter(float x)
{
	float v1, y;





	//YOUR CODE HERE
	// is it All Pass or Comb Filter?
	if(mode == ALLPASS){
		//* implement filter
		v1 = x + (a * (*last));
		y = (-b * v1) + (*last);

	}
	else if(mode == COMB){
		//* implement filter
		v1 = (x + (-a * (*last)));
		y = b * v1;
	}

	// shift delay line circular buffer
	if(--last < bottom){
		last = top;
	}
	if(--first < bottom){
		first = top;
	}

// write newest value into delay line buffer
	*first = v1;

	//return filter output
	return y;
}

//zero delay line
void paraReverb::clear(void)
{
	for (int i=0; i<m; i++) {
		buf[i] = 0.0;
	}
}
