//
// Created by qing on 18-12-26.
//
#include "IDecode.h"
#include "../log/XLog.h"

class DecodeObserver : public IObserver {
public:
    void Update(XData d) {
//        XLOGE("===========Update: %d",d.decodeType);
        if (d.decodeType != decode->decodeType) { //发送过来的数据类型与解码器数据类型不相同，则不处理
            return;
        }
        //阻塞（达到阻塞解封装器线程的目的）
        while (!decode->isExit) { //线程未结束
            decode->packetMutex.lock();
            if (decode->packs.size() < decode->maxList) {
                decode->packs.push_back(d);
                decode->packetMutex.unlock();
                break;
            }
            decode->packetMutex.unlock();
            XSleep(1);
        }
    };

    IDecode *decode;

    DecodeObserver(IDecode *deco) {
        this->decode = deco;
    };
};

void IDecode::Clear() {
    packetMutex.lock();
    while (!packs.empty()) {
        packs.front().Drop();
        packs.pop_front();
        XSleep(1);
    }
    pts = 0;
    synPts = 0;
    packetMutex.unlock();
}

//当前时间戳
long long GetNowMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    //秒
    int sec = tv.tv_sec % 360000; //只计算100个小时以内的毫秒值
    long long t = sec * 1000 + tv.tv_usec / 1000;
    return t;
}

void IDecode::Main() {
    long long startTime = GetNowMs();
    while (!isExit) {
        if (IsPausing()) {
            XSleep(2);
            continue;
        }
//        XLOGE("IDecode::Main()");
        packetMutex.lock();
        //超过3秒
//        if ((GetNowMs() - startTime) >= 3000) {
//            if (decodeType == DECODE_TYPE_VIDEO) {
//                XLOGE("视频 decode fps is %d", frameCount / 3);
//            } else {
//                XLOGE("音频 decode fps is %d", frameCount / 3);
//            }
//            startTime = GetNowMs();
//            frameCount = 0;
//        }
        //判断音视频同步 根据音频播放位置synPts控制视频解码位置
        if (decodeType == DECODE_TYPE_VIDEO) {
            // 音频播放的位置　小于当前视频解码位置,视频解码暂停
            if (synPts > 0 && synPts < pts) {
                packetMutex.unlock();
                XSleep(1);
                continue;
            }
        }
        if (packs.empty()) {
            packetMutex.unlock();
            XSleep(1);
            continue;
        }
        XData packet = packs.front();
        packs.pop_front();
        if (this->SendPacket(packet)) { //发送数据到解码线程成功

            while (!isExit) { // SendPacket发送一次，ReceivedFrame多次读取
                //获取解码数据
                XData frame = this->ReceivedFrame();
                if (!frame.data) { //数据读取不到了，就退出循环
                    break;
                }
                frameCount++;
                pts = frame.pts;
//                XLOGE("======>> %d", frame.size);
                //发送解码结果
                observable->Notify(frame);
            }
        }
        packet.Drop();
        packetMutex.unlock();
    }
}

IDecode::IDecode() {
    this->observer = new DecodeObserver(this);
}