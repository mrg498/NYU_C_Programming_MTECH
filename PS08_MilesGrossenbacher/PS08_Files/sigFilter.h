#ifndef _FILTER_H
#define _FILTER_H

#define NUM_CHN 2
#define COEFF 28

class sigFilter : public baseEffect {
public:
    sigFilter();
    ~sigFilter();
    /* virtual functions */
    void init(int mode);
    void process(float *ibuf, float *obuf, int num_frames, int num_chn);

    float* h;

    int len_filt;
    float state[NUM_CHN*COEFF];

};
#endif
