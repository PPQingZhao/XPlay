//
// Created by qing on 19-1-21.
//

#ifndef XPLAY_IAUDIOPLAY_H
#define XPLAY_IAUDIOPLAY_H


#include <list>
#include <mutex>
#include "../observer/IObserver.h"
#include "../data/XParameter.h"
#include "../thread/XThread.h"

class IAudioPlay:public XThread{

public:
    virtual bool StartPlay(XParameter out) = 0;
    virtual void Close() = 0;
    virtual void Clear();
    //获取缓冲数据,没有则阻塞
    virtual XData GetData();

    IObserver *audioObserver;

    IAudioPlay();

    std::list<XData> frames;
    std::mutex frameMutex;
    //最大缓冲
    int maxFrame = 100;
    int pts = 0;
};


#endif //XPLAY_IAUDIOPLAY_H
