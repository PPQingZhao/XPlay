//
// Created by qing on 18-10-23.
//

#ifndef XPLAY_XDATA_H
#define XPLAY_XDATA_H

enum XDecodeFormat {  //　视频解码格式
    DECODE_HARD,  //硬解码
    DECODE_SOFT,  //软解码
};

enum XDataType {
    AVPACKET_TYPE = 0, //avpacket
    UCHAR_TYPE = 1,    //普通类型
};

struct XData {
    unsigned char *data = 0;
    unsigned char *datas[8] = {0};
    int size = 0;
    int decodeType;                     //解码数据类型：　音频数据，视频数据，字幕数据
    int width = 0;
    int height = 0;
    int packetType;                     //数据包类型 详情参考枚举XDataType
    int format = 0;                     //视频解码格式
    bool Alloc(int size, const char *data = 0);

    void Drop();
};


#endif //XPLAY_XDATA_H
