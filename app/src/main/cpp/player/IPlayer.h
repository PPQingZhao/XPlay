//
// Created by qing on 19-3-5.
//

#ifndef XPLAY_IPLAYER_H
#define XPLAY_IPLAYER_H


#include "../thread/XThread.h"
#include "../data/XParameter.h"

class IDemux;

class IDecode;

class IResample;

class IVideoView;

class IAudioPlay;


class IPlayer : public XThread {
public:
    static IPlayer *Get(unsigned char index = 0);

    virtual bool Open(const char *path);

    virtual bool Start();

    virtual bool InitView(void *win);

    IDemux *demux = 0;
    IDecode *vDecode = 0;
    IDecode *aDecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;
    //音频输出参数配置
    XParameter aOutPara;
protected:

    IPlayer() {};
};


#endif //XPLAY_IPLAYER_H
