#include <stdio.h>
#include <math.h>
#include <sndfile.h>	/* libsndfile */
#include "wav_reverb.h"
#include "convolve.h"	/* FFT-based convolution */

#define MODE	2

void process_audio(Buf *ibuf, int iframes, int channels, Buf *rbuf, int rframes, Buf *obuf)
{
#if (MODE == 1)
	/* just copy input to output */
	int i, j;
	//float *ip, *rp;
	/* for each channel and each frame */
	for(i = 0; i < iframes; i++){
		for(j = 0; j < channels; j++){
			if(i < iframes){
				obuf->buf[j][i] = ibuf->buf[j][i];
			}
			else{
				obuf->buf[j][i] = rbuf->buf[j][i];
			}
		}
	}
	return;

#else
#if (MODE == 2)
	/* do reverberation via time-domain convolution */
	int i, j, k, oframes;
	//float *ip, *rp;
	double v, rms_iv, rms_ov;
	/* set initial values */
	rms_iv = 0;
	rms_ov = 0;
	// ToDo: convolve each channel signal with reverb impulse response signal
	oframes = iframes + rframes - 1;

	for(i = 0; i < oframes; i++){
		for(j = 0; j < channels; j++){
			obuf->buf[j][i] = 0;
			for(k = 0; k < rframes; k++){
				if((i-k) >= 0 && (i-k) <= (iframes -1)){
					obuf->buf[j][i] += ibuf->buf[j][i-k] * rbuf->buf[j][k];
				}
			}
		}
	}

	//scale to make output rms value be equal to input rms value
	for(int i = 0; i < iframes; i++){
		for(int j = 0; j < channels; j++){
			rms_iv += ibuf->buf[j][i] * ibuf->buf[j][i];
		}
	}
	rms_iv = sqrt(rms_iv/iframes);

	for(int i = 0; i < oframes; i++){
		for(int j = 0; j < channels; j++){
			rms_ov += obuf->buf[j][i] * obuf->buf[j][i];
		}
	}
	rms_ov = sqrt(rms_ov/oframes);

	v = rms_iv/rms_ov;

	for(int i = 0; i < oframes; i++){
		for(int j = 0; j < channels; j++){
			obuf->buf[j][i] = obuf->buf[j][i] * v;
		}
	}


#else /* (MODE == 3) */
	/* do reverberation via frequency-domain convolution */
	int i;
	/* for each channel */
	for (i=0; i<channels; i++) {
		/* convolve channel signal with reverb impulse response signal */
		/* frequency domain (FFT) convolution */
		convolve(ibuf->buf[i], rbuf->buf[i], iframes, rframes, obuf->buf[i]);
	}
#endif
#endif
}
