//
// Created by qing on 18-12-26.
//

#ifndef XPLAY_XPARAMETER_H
#define XPLAY_XPARAMETER_H

struct AVCodecParameters;

class XParameter {
public:
    AVCodecParameters *para;
    int channels = 2;        //通道数(音频)
    int sample_rate = 44100; //样本率(音频)
};


#endif //XPLAY_XPARAMETER_H
