//
// Created by qing on 19-1-21.
//

#include "IAudioPlay.h"
#include "../observer/IObserver.h"
#include "../thread/XThread.h"
#include "../log/XLog.h"

class AudioObserver : public IObserver {
public:
    IAudioPlay *audioPlay;

    AudioObserver(IAudioPlay *audioPlay) {
        this->audioPlay = audioPlay;
    }

    //缓冲阻塞
    void Update(XData data) {
        if (!audioPlay) return;
        //压入缓冲队列
        if (data.size <= 0 || !data.data) {
            return;
        }
//        XLOGE("AudioPlay update size is %d.",data.size);
        while (true) {
//            XLOGE("AudioPlay update maxFrame size is %d ", audioPlay->maxFrame);
            audioPlay->frameMutex.lock();
            if (audioPlay->frames.size() > audioPlay->maxFrame) {
//                XLOGE("AudioPlay update22 size is %d.",data.size);
                audioPlay->frameMutex.unlock();
                XSleep(1);
                continue;
            }
            audioPlay->frames.push_back(data);
            audioPlay->frameMutex.unlock();
            break;
        }
    }

};

void IAudioPlay::Clear() {
    frameMutex.lock();
    while (!frames.empty()) {
        frames.front().Drop();
        frames.pop_front();
    }
    frameMutex.unlock();
}

XData IAudioPlay::GetData() {
    XData d;
    isRunning = true;
    while (!isExit) {
        if (IsPausing()) {
            XSleep(2);
            continue;
        }
//        XLOGE("AudioPlay GetData is %d ", frames.empty());
        frameMutex.lock();
        if (!frames.empty()) {
//            XLOGE("AudioPlay GetData22 is %d ", frames.empty());
            d = frames.front();
            frames.pop_front();
            pts = d.pts;
            frameMutex.unlock();
            return d;
        }
        frameMutex.unlock();
        XSleep(1);
    }
    isRunning = false;
    //未获取数据
    return d;
}

IAudioPlay::IAudioPlay() {
    this->audioObserver = new AudioObserver(this);
}