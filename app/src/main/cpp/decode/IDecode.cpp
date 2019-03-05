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

void IDecode::Main() {
    while (!isExit) {
        packetMutex.lock();
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