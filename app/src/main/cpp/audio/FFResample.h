//
// Created by qing on 19-1-21.
//

#ifndef XPLAY_FFRESAMPLE_H
#define XPLAY_FFRESAMPLE_H


#include "IResample.h"

struct SwrContext;

class FFResample : public IResample {
public:
    virtual bool Open(XParameter in, XParameter out = XParameter());

    virtual XData Resample(XData indata);

protected:
    //音频上下文
    SwrContext *swrContext = 0;
};


#endif //XPLAY_FFRESAMPLE_H
