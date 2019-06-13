//
// Created by qing on 18-10-23.
//

#ifndef XPLAY_FFDEMUX_H
#define XPLAY_FFDEMUX_H

#include "IDemux.h"

struct AVFormatContext;

class FFDemux : public IDemux {
public:
    //打开文件，或者流媒体　rmtp http rtsp
    virtual bool Open(const char *url);
    virtual void Close();

    //seek位置 pos 0.0-1.0
    virtual bool Seek(double pos);

    //获取参数
    virtual XParameter GetVideoParameter();
    //获取音频帧参数
    virtual XParameter GetAudioParameter();

    //读取一帧数据，数据由调用者清理
    virtual XData Read();

    FFDemux();

private:
    bool isFirst = true;
    AVFormatContext *avFormatContext = NULL;
    std::mutex mux;
    int audioStream = 1;  //音频流索引
    int videoStream = 0;  //视频流索引
};

#endif //XPLAY_FFDEMUX_H
