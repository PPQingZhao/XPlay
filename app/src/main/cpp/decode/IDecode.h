//
// Created by qing on 18-12-26.
//

#ifndef XPLAY_IDECODE_H
#define XPLAY_IDECODE_H

#include "../data/XParameter.h"
#include "../data/XData.h"
#include "../thread/XThread.h"
#include "../observer/IObservable.h"
#include <list>

//解码接口　支持硬解码
class IDecode : public XThread {
public:
    /**
      *　打开视频解码器
      * @param xParameter　　
      * @param format 　　　　//解码格式,默认软解码
      * @return
      */
    virtual bool Open(XParameter xParameter, XDecodeFormat format = DECODE_SOFT) = 0;

    virtual void Close() = 0;

    virtual void Clear();

    //发送数据到解码线程
    virtual bool SendPacket(XData pkt) = 0;

    //从线程中接收解码结果，再次调用会复用上次空间,线程不安全
    virtual XData ReceivedFrame() = 0;

    //观察者,观察解封装器的数据
    IObserver *observer;
    //被观察者,解码器获取到解码结果后,需要通知外部的观察者
    IObservable *observable = new IObservable();
    XDecodeType decodeType; //当前解码类型: 音频,视频,字幕
    //读取缓冲
    std::list<XData> packs;
    std::mutex packetMutex; //互斥处理
    int maxList = 100; //最大缓冲
    //音视频同步时间　，再次打开文件需要清理：置０
    int synPts = 0;
    int pts = 0;
    //是否打开解码器
    bool isOpen;
    int frameCount = 0;

    IDecode();

protected:
    virtual void Main();
};

#endif //XPLAY_IDECODE_H
