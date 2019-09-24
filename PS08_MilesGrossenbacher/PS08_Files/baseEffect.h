#ifndef _BASE_EFFECT_H
#define _BASE_EFFECT_H

class baseEffect {
public:
    baseEffect();
    ~baseEffect();
    /* virtual functions */
    virtual void init(int mode) {;}
    virtual void process(float *ibuf, float *obuf, int num_frames, int num_chn) {;}
};

#endif
