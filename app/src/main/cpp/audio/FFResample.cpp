//
// Created by qing on 19-1-21.
//
extern "C" {
#include <libswresample/swresample.h>
}

#include <libavcodec/avcodec.h>
#include "FFResample.h"
#include "../log/XLog.h"

bool FFResample::Open(XParameter in, XParameter out) {
    //音频重采样上下文初始化
    swrContext = swr_alloc();
    swrContext = swr_alloc_set_opts(swrContext,
                                    av_get_default_channel_layout(out.channels),
                                    AV_SAMPLE_FMT_S16,
                                    out.sample_rate,
                                    av_get_default_channel_layout(in.para->channels),
                                    (AVSampleFormat) in.para->format,
                                    in.para->sample_rate,
                                    0,
                                    0);
    int ret = swr_init(swrContext);
    if (ret != 0) {
        XLOGE("swr_init init failed!");
        return false;
    }
    outChannels = in.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    XLOGE("swr_init init success!");
    return true;
}

XData FFResample::Resample(XData indata) {
    if (indata.size <= 0 || !indata.data) return XData();
    if (!swrContext)
        return XData();
    AVFrame *frame = (AVFrame *) indata.data;
    //输出空间的分配
    XData out;
    //通道数* 单通道样本数 * 样本字节大小
    int outSize = outChannels * frame->nb_samples * av_get_bytes_per_sample(
            (AVSampleFormat) outFormat);
    if (outSize <= 0)return XData();
    out.Alloc(outSize);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(swrContext,
                          outArr,
                          frame->nb_samples,
                          (const uint8_t **) frame->data,
                          frame->nb_samples);
    if (len <= 0) {
        out.Drop();
        return XData();
    }
//    XLOGE("============>>swr_convert success! ");
    return out;
}