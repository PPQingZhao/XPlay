//
// Created by qing on 18-10-23.
//

#include <stdio.h>
#include "FFDemux.h"
#include "../log/XLog.h"

extern "C" {
#include <libavformat/avformat.h>
}

//打开文件，或者流媒体　rmtp http rtsp
bool FFDemux::Open(const char *url) {
    XLOGE("================>> open file %s begin!", url);

    //打开文件
    int ret = avformat_open_input(&avFormatContext, url, NULL, NULL);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("=============>> avformat_open_input failed!");
    }

    //读取文件信息
    ret = avformat_find_stream_info(avFormatContext, NULL);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("=============>> avformat_find_stream_info %s failed!", url);
    }
    this->totalMs = avFormatContext->duration / (AV_TIME_BASE / 1000);
    XLOGE("===========>> totalTime: %lld", this->totalMs);
    XLOGE("=============>> FFDemux success!");
    GetAudioParameter();
    GetVideoParameter();
    return true;
}

//获取视频参数
XParameter FFDemux::GetVideoParameter() {
    if (!avFormatContext) {
        XLOGE("GetVideoParameter failed!");
        return XParameter();
    }
    //获取视频流索引
    int ret = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (ret < 0) {
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    XLOGE("av_find_best_stream success!");
    videoStream = ret;
    XParameter xParameter;
    xParameter.para = avFormatContext->streams[ret]->codecpar;
    return xParameter;
}

//获取音频参数
XParameter FFDemux::GetAudioParameter() {
    if (!avFormatContext) {
        XLOGE("GetVideoParameter failed!");
        return XParameter();
    }
    //获取音频流索引
    int ret = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (ret < 0) {
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    XLOGE("av_find_best_stream success!");
    audioStream = ret;
    XParameter xParameter;
    xParameter.para = avFormatContext->streams[ret]->codecpar;
    xParameter.channels = avFormatContext->streams[ret]->codecpar->channels;
    xParameter.sample_rate = avFormatContext->streams[ret]->codecpar->sample_rate;
    return xParameter;
}

//读取一帧数据，数据由调用者清理
XData FFDemux::Read() {
    if (!avFormatContext)
        return XData();

    XData data;
    AVPacket *avPacket = av_packet_alloc();
    int ret = av_read_frame(avFormatContext, avPacket);
    if (ret != 0) {
        //释放内存
        av_packet_free(&avPacket);
        return XData();
    }

    data.data = (unsigned char *) avPacket;
    data.size = avPacket->size;
    if (avPacket->stream_index == videoStream) {
        data.decodeType = AVMEDIA_TYPE_VIDEO; //视频数据
    } else if (avPacket->stream_index == audioStream) {
        data.decodeType = AVMEDIA_TYPE_AUDIO; //音频数据
    } else {
        av_packet_free(&avPacket);
        return XData();
    }
//    XLOGE("=======>> size: %d  pts: %d ", avPacket->size, avPacket->pts);
    return data;
}

FFDemux::FFDemux() {
    if (isFirst) {
        isFirst = false;
        //ffmpeg 注册所有封装器
        av_register_all();
        //注册所有的解码器
        avcodec_register_all();
        //初始化网络
        avformat_network_init();
        XLOGE("================>> register info");
    }
}