#include "baseEffect.h"
#include "passThrough.h"

passThrough::passThrough(void) {;}
passThrough::~passThrough() {;}
void passThrough::init(int mode) {;}
void passThrough::process(float *ibuf, float *obuf, int num_frames, int num_chn)
{
    /* copy input to output */
    for (int i=0; i<num_frames; i++) {
        for (int j=0; j<num_chn; j++) {
            obuf[i*num_chn + j] = ibuf[i*num_chn +j];
        }
    }
}
