//
// Created by qing on 19-1-21.
//

#ifndef XPLAY_SLAUDIOPLAY_H
#define XPLAY_SLAUDIOPLAY_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay {
public:
    virtual bool StartPlay(XParameter out);
    virtual void Close();
    void PlayCall(void *bufq);
    SLEngineItf CreateSL();
    SLAudioPlay();
    virtual ~SLAudioPlay();
protected:
    unsigned char *buf = 0;
    std::mutex mux;
    SLObjectItf slObjectItf = NULL;
    SLEngineItf slEngineItf;
    SLObjectItf mix = NULL;
    SLObjectItf player = NULL;
    SLPlayItf slPlayItf = NULL;
    SLAndroidSimpleBufferQueueItf simpleBufferQueueItf_player = NULL;
};


#endif //XPLAY_SLAUDIOPLAY_H
