//
// Created by qing on 18-10-23.
//

#include "XData.h"

extern "C" {
#include <libavformat/avformat.h>
}

bool XData::Alloc(int size, const char *d) {
    Drop();
    packetType = UCHAR_TYPE;
    if (size <= 0)return false;
    this->data = new unsigned char[size];
    if (!this->data) return false;
    if (d){
        memcpy(this->data,d,size);
    }
    this->size = size;
    return true;
}

void XData::Drop() {

    if (!data) return;

    if (packetType == AVPACKET_TYPE) {                  //释放avpacket类型
        av_packet_free((AVPacket **) &data);
    } else {
        delete data;
    }
    data = 0;
    size = 0;
}