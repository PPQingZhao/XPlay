//
// Created by qing on 19-3-5.
//

#ifndef XPLAY_IPLAYER_H
#define XPLAY_IPLAYER_H


#include "../thread/XThread.h"

class IDemux;

class IDecode;

class IResample;

class IVideoView;

class IAudioPlay;

class IPlayer : public XThread {
public:
    static IPlayer *Get(unsigned char index);

    virtual bool Open(const char *path);
    virtual bool Start();

    IDemux *demux = 0;
    IDecode *vDecode = 0;
    IDecode *aDecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;

protected:

    IPlayer();
};


#endif //XPLAY_IPLAYER_H
