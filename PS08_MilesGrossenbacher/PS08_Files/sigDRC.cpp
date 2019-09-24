#include "baseEffect.h"
#include "sigDRC.h"

sigDRC::sigDRC(void) {;}
sigDRC::~sigDRC() {;}
void sigDRC::init(int mode)
{

}
void sigDRC::process(float *ibuf, float *obuf, int num_frames, int num_chn)
{

    float x_low[] = {0, 0.25};
    float x_high[] = {0.25, 1};
    float a[] = {3, 1/3};
    float b[] = {0, 0.75};
    // we want to access every samples in every channel
    for (int n=0; n<num_frames; n++) {
        for (int j=0; j<num_chn; j++) {
          // we want to determine if the sample amp is positive or negative
          int s_flag = (ibuf[n*num_chn + j] < 0) ? -1:1;
          // use the sign flag to make the value positive
          frame_abs = ibuf[n*num_chn + j] * s_flag;

          // determine which line segment slope we want to use
          which_line = (frame_abs < x_high[0]) ? 0:1;

          //use the equation we were given to alter the frame and put it into the obuf
          obuf[n*num_chn + j] = s_flag * (a[which_line] * (frame_abs - x_low[which_line]) + b[which_line]);
        }
    }
}
