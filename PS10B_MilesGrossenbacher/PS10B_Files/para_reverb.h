#ifndef	PREVERB_H
#define PREVERB_H

#define ALLPASS	1
#define COMB	2

class paraReverb {
private:
	int mode; 	//APF or FBCF
	int m; 		//delay line length
	float a; 	//coefficients
	float b;
	float *buf; //pointer to delay line
	float *bottom, *top, *first, *last;
public:
	//constructor
	paraReverb(int mode, float a, float b, int m);
	//destructor
	~paraReverb();
	//methods
	float filter(float x); //filter one value
	void clear(void); //clear filter state
};

#endif