//
// Created by qing on 18-12-26.
//

#ifndef XPLAY_FFDECODE_H
#define XPLAY_FFDECODE_H

#include <libavcodec/avcodec.h>
#include "IDecode.h"

struct AVCodecContext;

class FFDecode : public IDecode {
public:
    static void InitHard(void *vm);
    /**
      *　打开视频解码器
     * @param xParameter　　
     * @param format 　　　　//解码格式
     * @return
     */
    virtual bool Open(XParameter xParameter,XDecodeFormat format);
    virtual void Close();
    virtual void Clear();
    //发送数据到线程
    virtual bool SendPacket(XData data);

    //从线程中接收数据
    virtual XData ReceivedFrame();

protected:
    AVCodecContext *avCodecContext = 0;
    AVFrame *avFrame = 0;
    std::mutex mux;
};

#endif //XPLAY_FFDECODE_H
