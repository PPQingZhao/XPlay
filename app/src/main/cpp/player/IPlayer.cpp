//
// Created by qing on 19-3-5.
//

#include "IPlayer.h"
#include "../demux/IDemux.h"
#include "../decode/IDecode.h"
#include "../audio/IResample.h"
#include "../videoviewer/IVideoView.h"
#include "../audio/IAudioPlay.h"
#include "../log/XLog.h"

IPlayer *IPlayer::Get(unsigned char index) {
    //创建256个player
//    static IPlayer iPlayer[256];
     IPlayer *player = new IPlayer();
//    return &iPlayer[index];
    return player;
}

bool IPlayer::Open(const char *path) {
    //解封装
    if (!demux || !demux->Open(path)) {
        XLOGE("demux->Open failed %s", path);
        return false;
    }
    //视频解码．解码可能不需要(如果解封后就是原始数据)
    if (!vDecode || !vDecode->Open(demux->GetVideoParameter())) {
        XLOGE("vDecode->Open fialed %s ", path);
//        return false;
    }
    if (!aDecode || !aDecode->Open(demux->GetAudioParameter())) {
        XLOGE("aDecode->Open fialed %s ", path);
//        return false;
    }
    //重采样　有可能不需要，解码或者解封后就是直接能播放的数据
    if (aOutPara.sample_rate <= 0) {
        aOutPara = demux->GetAudioParameter();
    }
    if (!resample || !resample->Open(demux->GetAudioParameter(), aOutPara)) {
        XLOGE("resample->Open fialed %s ", path);
    }
    return true;
}

bool IPlayer::Start() {
    //启动解封装线程
    if (!demux || !demux->Start()) {
        XLOGE("demux->Start() failed");
        return false;
    }
    //先启动音频解码，再次启动视频解码
    if (aDecode) {
        aDecode->Start();
    }
    if (audioPlay) {
        audioPlay->StartPlay(aOutPara);
    }
    if (vDecode) {
        vDecode->Start();
    }
    return true;
}

bool IPlayer::InitView(void *win) {
    if (videoView) {
        videoView->SetRender(win);
    }
    return true;
}