//
// Created by qing on 19-3-5.
//

#ifndef XPLAY_IPLAYER_H
#define XPLAY_IPLAYER_H


#include <mutex>
#include "../thread/XThread.h"
#include "../data/XParameter.h"
#include "../data/XData.h"

class IDemux;

class IDecode;

class IResample;

class IVideoView;

class IAudioPlay;


class IPlayer : public XThread {
public:
    static IPlayer *Get();

    virtual bool Open(const char *path);

    virtual bool Start();

    virtual void Close();

    virtual bool Seek(double pos);

    virtual bool InitView(void *win);

    //获取当前播放进度 0.0~1.0
    double PlayPos();

    void SetPause(bool isP);

    IDemux *demux = 0;
    IDecode *vDecode = 0;
    IDecode *aDecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;
    //音频输出参数配置
    XParameter aOutPara;
    bool isHardDecode = false;
protected:
    //用作音视频同步
    void Main();

    std::mutex mutex;

    IPlayer() {};
};


#endif //XPLAY_IPLAYER_H
