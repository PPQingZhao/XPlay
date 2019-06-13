
#include <stdio.h>
#include "FFDemux.h"
#include "../log/XLog.h"

extern "C" {
#include <libavformat/avformat.h>
}

bool FFDemux::Seek(double pos) {
    XLOGE(" FFDemux::Seek");
    if (pos < 0 || pos > 1) {
        XLOGE("Seek value must 0.0-1.0! pos: %d", pos);
        return false;
    }
    bool ret = false;
    mux.lock();
    if (!avFormatContext) {
        mux.unlock();
        return false;
    }
    //清理读取的缓冲
    avformat_flush(avFormatContext);
    long long seekPts = 0;
    seekPts = avFormatContext->streams[videoStream]->duration * pos;
    XLOGE(" FFDemux::Seek22");
    //往后跳转到关键帧
    av_seek_frame(avFormatContext, videoStream, seekPts, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);
    mux.unlock();
    return ret;
}

void FFDemux::Close() {
    mux.lock();
    if (avFormatContext) {
        avformat_close_input(&avFormatContext);
    }
    isOPen = false;
    mux.unlock();
}

//打开文件，或者流媒体　rmtp http rtsp
bool FFDemux::Open(const char *url) {
    XLOGE("================>> open file %s begin!", url);
    //打开文件
    Close();
    mux.lock();
    int ret = avformat_open_input(&avFormatContext, url, 0, 0);
    if (ret != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("=============>> avformat_open_input failed!");
        return false;
    }

    //读取文件信息
    ret = avformat_find_stream_info(avFormatContext, 0);
    if (ret != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("=============>> avformat_find_stream_info %s failed!", url);
        return false;
    }
    isOPen = true;
    this->totalMs = avFormatContext->duration / (AV_TIME_BASE / 1000);

    XLOGE("===========>> totalTime: %lld", this->totalMs);
    XLOGE("=============>> FFDemux success!");
    mux.unlock();
    return true;
}

//获取视频参数
XParameter FFDemux::GetVideoParameter() {
    mux.lock();
    if (!avFormatContext) {
        mux.unlock();
        XLOGE("GetVideoParameter failed!");
        return XParameter();
    }
    //获取视频流索引
    int ret = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (ret < 0) {
        mux.unlock();
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    XLOGE("GetVideoParameter av_find_best_stream success!");
    videoStream = ret;
    XParameter xParameter;
    xParameter.para = avFormatContext->streams[ret]->codecpar;
    mux.unlock();
    return xParameter;
}

//获取音频参数
XParameter FFDemux::GetAudioParameter() {
    mux.lock();
    if (!avFormatContext) {
        XLOGE("GetVideoParameter failed!");
        mux.unlock();
        return XParameter();
    }
    //获取音频流索引
    int ret = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (ret < 0) {
        XLOGE("av_find_best_stream failed!");
        mux.unlock();
        return XParameter();
    }
    XLOGE("GetAudioParameter av_find_best_stream success! %d ", ret);
    audioStream = ret;
    XParameter xParameter;
    xParameter.para = avFormatContext->streams[ret]->codecpar;
    xParameter.channels = avFormatContext->streams[ret]->codecpar->channels;
    xParameter.sample_rate = avFormatContext->streams[ret]->codecpar->sample_rate;
    mux.unlock();
    return xParameter;
}

//分数转浮点数
static double r2d(AVRational rational) {
    return rational.num == 0 | rational.den == 0 ? 0 : (double) rational.num /
                                                       (double) rational.den;
}

//读取一帧数据，数据由调用者清理
XData FFDemux::Read() {
    mux.lock();
    if (!avFormatContext) {
        mux.unlock();
        return XData();
    }

    XData data;
    AVPacket *avPacket = av_packet_alloc();
    int ret = av_read_frame(avFormatContext, avPacket);
    if (ret != 0) {
        mux.unlock();
        //释放内存
        av_packet_free(&avPacket);
        return XData();
    }
    //转换pts
    avPacket->pts = avPacket->pts *
                    (1000 * r2d(avFormatContext->streams[avPacket->stream_index]->time_base));
    avPacket->dts = avPacket->dts *
                    (1000 * r2d(avFormatContext->streams[avPacket->stream_index]->time_base));
    data.data = (unsigned char *) avPacket;
    data.size = avPacket->size;

    if (avPacket->stream_index == videoStream) {
        data.decodeType = DECODE_TYPE_VIDEO; //视频数据
    } else if (avPacket->stream_index == audioStream) {
        data.decodeType = DECODE_TYPE_AUDIO; //音频数据
    } else {
        mux.unlock();
        av_packet_free(&avPacket);
        return XData();
    }
    mux.unlock();
//    XLOGE("***********  data.pts: %ld", data.pts);
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