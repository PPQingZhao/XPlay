//
// Created by qing on 18-10-23.
//

#ifndef XPLAY_XDATA_H
#define XPLAY_XDATA_H

enum XDecodeFormat {  //　视频解码格式
    DECODE_HARD = 0,  //硬解码
    DECODE_SOFT = 1,  //软解码
};

enum XDecodeType {
    DECODE_TYPE_UNKNOWN = -1,   //< Usually treated as AVMEDIA_TYPE_DATA
    DECODE_TYPE_VIDEO = 0,      // 视频解码
    DECODE_TYPE_AUDIO = 1,      //　音频解码
};

enum XDataType {
    AVPACKET_TYPE = 0, //avpacket
    UCHAR_TYPE = 1,    //普通类型
};

struct XData {
    int pts = 0;
    unsigned char *data = 0;
    unsigned char *datas[8] = {0};
    int size = 0;
    XDecodeType decodeType;                     //解码数据类型：　音频数据，视频数据，字幕数据　参考枚举XDecodeType
    int width = 0;
    int height = 0;
    int packetType;                     //数据包类型 详情参考枚举XDataType
    int format = 0;                     //视频解码格式
    bool Alloc(int size, const char *data = 0);

    void Drop();
};


#endif //XPLAY_XDATA_H
