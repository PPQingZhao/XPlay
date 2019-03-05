//
// Created by qing on 18-10-23.
//

#ifndef XPLAY_IDEMUX_H
#define XPLAY_IDEMUX_H

#include "../data/XData.h"
#include "../thread/XThread.h"
#include "../observer/IObservable.h"
#include "../data/XParameter.h"

/*解封装接口*/
class IDemux : public XThread {
public:
    //打开文件，或者流媒体　rmtp http rtsp
    virtual bool Open(const char *url) = 0;  //纯虚函数
    //获取视频帧参数
    virtual XParameter GetVideoParameter() = 0;

    //获取音频帧参数
    virtual XParameter GetAudioParameter() = 0;

    //读取一帧数据，数据由调用者清理
    virtual XData Read() = 0;

    IObservable *observable = new IObservable();
    //总时长(毫秒)
    int totalMs = 0;
protected:
    virtual void Main();

    IDemux() {

    };
};

#endif //XPLAY_IDEMUX_H
