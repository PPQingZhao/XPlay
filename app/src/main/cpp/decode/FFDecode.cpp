
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
}

#include "FFDecode.h"
#include "../log/XLog.h"

void FFDecode::InitHard(void *vm) {
    int ret = av_jni_set_java_vm(vm, NULL);
    if (0 != ret) {
        XLOGE("av_jni_set_java_vm failed!");
        return;
    }
    XLOGE("av_jni_set_java_vm success!");
};

void FFDecode::Clear() {
    //①先清理父类中本地缓冲队列 packs
    IDecode::Clear();
    mux.lock();
    //②清理ffmpeg缓冲队列
    if (avCodecContext) {
        avcodec_flush_buffers(avCodecContext);
    }
    mux.unlock();
}

void FFDecode::Close() {
    IDecode::Clear();
    mux.lock();
    pts = 0;
    if (avFrame) {
        av_frame_free(&avFrame);
    }
    if (avCodecContext) {
        avcodec_close(avCodecContext);
        avcodec_free_context(&avCodecContext);
    }
    isOpen = false;
    mux.unlock();
}

bool FFDecode::Open(XParameter xParameter, XDecodeFormat format) {
    Close();
    if (!xParameter.para) {
        XLOGE("FFDecode Open failed!");
        return false;
    }
    AVCodecParameters *avCodecParameters = xParameter.para;
    //①查找解码器
    AVCodec *avCodec;
    switch (format) {
        case DECODE_HARD:  //硬解码
            avCodec = avcodec_find_decoder_by_name("h264_mediacodec");
            XLOGE("DECODE_HARD format: %d", format);
            break;
        case DECODE_SOFT: // 软解码
            avCodec = avcodec_find_decoder(avCodecParameters->codec_id);
            XLOGE("DECODE_SOFT format: %d", format);
            break;
    }
    //查找失败
    if (!avCodec) {
        XLOGE("avcodec_find_decoder %d failed! %d", avCodecParameters->codec_id, format);
        return false;
    }
    mux.lock();
    //②.创建解码上下文,并复制参数
    avCodecContext = avcodec_alloc_context3(avCodec);
    avcodec_parameters_to_context(avCodecContext, avCodecParameters);
    //开启8个线程
    avCodecContext->thread_count = 8;
    //③打开解码器
    int ret = avcodec_open2(avCodecContext, 0, 0);
    if (ret != 0) {
        XLOGE("avcodec_open2 failed!");
        char errBuf[1024] = {0};
        av_strerror(ret, errBuf, sizeof(errBuf) - 1);
        XLOGE("%s", errBuf);
        mux.unlock();
        return false;
    }
    if (avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        this->decodeType = DECODE_TYPE_VIDEO;
    } else if (avCodecContext->codec_type == AVMEDIA_TYPE_AUDIO) {
        this->decodeType = DECODE_TYPE_AUDIO;
    } else {
        this->decodeType = DECODE_TYPE_UNKNOWN;
    }
    isOpen = true;
    mux.unlock();
    XLOGE("FFDecode::Open success! %d", avCodecParameters->codec_id);
    return true;
}

bool FFDecode::SendPacket(XData pkt) {
    if (!avCodecContext || !pkt.data) {
        XLOGE("FFDecode::SendPacket failed!");
        return false;
    }
    mux.lock();
    if (!avCodecContext) {
        mux.unlock();
        return false;
    }
    int ret = avcodec_send_packet(avCodecContext, (const AVPacket *) pkt.data);
    mux.unlock();
    if (ret != 0) {
        XLOGE("avcodec_send_packet failed! %s", av_err2str(ret));
        return false;
    }
    return true;
}

XData FFDecode::ReceivedFrame() {
    mux.lock();
    if (!avCodecContext) {
        XLOGE("FFDecodec::Recevied failed!");
        mux.unlock();
        return XData();
    }

    if (!avFrame) {
        avFrame = av_frame_alloc();
    }
    int ret = avcodec_receive_frame(avCodecContext, avFrame);
    if (ret != 0) {
//        XLOGE("avcodec_receive_frame failed");
        mux.unlock();
        return XData();
    }
    XData d;
    d.data = (unsigned char *) avFrame;
    d.format = avFrame->format;
    this->pts = d.pts = avFrame->pts;
    if (avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO) { //视频
        d.size = avFrame->linesize[0] + avFrame->linesize[1] +
                 avFrame->linesize[2] * avFrame->height;
        d.width = avFrame->width;
        d.height = avFrame->height;
//        XLOGE("AVMEDIA_TYPE_VIDEO data format is %d", avFrame->format);
    } else if (avCodecContext->codec_type == AVMEDIA_TYPE_AUDIO) { //音频
        //样本字节数*单通道样本数*通道数
        d.size =
                av_get_bytes_per_sample((AVSampleFormat) avFrame->format) * avFrame->nb_samples * 2;
    }
    memcpy(d.datas, avFrame->data, sizeof(d.datas));
    mux.unlock();
    return d;
}