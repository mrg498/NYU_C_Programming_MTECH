#ifndef _PASS_THROUGH_H //(why)
#define _PASS_THROUGH_H

class passThrough : public baseEffect {
public:
    passThrough();
    ~passThrough();
    /* virtual functions */
    void init(int mode);
    void process(float *ibuf, float *obuf, int num_frames, int num_chn);
};

#endif
