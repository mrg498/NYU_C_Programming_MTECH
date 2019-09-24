#include "baseEffect.h"
#include "sigFilter.h"
#include "hpf.h"
#include "lpf.h"
#include <stdio.h>
#include <stdlib.h>

sigFilter::sigFilter(void) {;}
sigFilter::~sigFilter() {;}
void sigFilter::init(int mode)
{
    if(mode == 0){
      //point to high-pass array
      h = hpf;
      len_filt = (sizeof(hpf)/sizeof(hpf[0]));

    }
    else{
      // point to low-pass array
      h = lpf;
      len_filt = (sizeof(lpf)/sizeof(lpf[0]));
      //printf("%f\n", h[0]);
      // this is working
    }

    //initialize state array
    // the size of this is the number of channels times the size of the filter array
    for(int i = 0; i < NUM_CHN * COEFF; i++){
      state[i] = 0.0;
    }

}
void sigFilter::process(float *ibuf, float *obuf, int num_frames, int num_chn)
{
  // get length of h

  // we index through all of the frames for each channel
  for(int n = 0; n < num_frames; n++){
    for(int j = 0; j<num_chn; j++){
      //for every sample, we do a summation of the multiplication against every
      // filter coefficient
      float sum = 0.0;
      for(int coef_index = 0; coef_index < len_filt; coef_index++){
        // if the filter doesn't need to pull any value from the previous callback ibuf
        // just calculate the summation
        if(n - coef_index >= 0){
          // we do the summation in reverse order starting
          sum += ibuf[(n - coef_index) * num_chn + j] * h[coef_index];
        }
        // if we must go back to values in the previous buffer, access the state buffer
        // and go back necessary amount and do the calculation against the coefficients
        else{
          sum+= state[(COEFF + (n-coef_index)) * NUM_CHN + j] * h[coef_index];
        }
      }
      // put the summation into the correct obuffer index
      obuf[n*num_chn + j] = sum;

    }
  }
  // after the obuffer has been completly filled
  // save the potential maximum samples that we will have to go back to in this ibuff
  // into a state buffer
  int counter = (num_frames - COEFF) * NUM_CHN;
  for(int saved_frames = 0; saved_frames < COEFF*NUM_CHN; ){
    state[saved_frames++] = ibuf[counter++];
  }
  return;
}
