
#include "IPlayerBuilder.h"
#include "../demux/IDemux.h"
#include "../decode/IDecode.h"
#include "../videoviewer/IVideoView.h"
#include "../audio/IResample.h"
#include "../audio/IAudioPlay.h"

IPlayer *IPlayerBuilder::BuilderPlayer() {
    IPlayer *player = CreateIplayer();
    //解封装
    IDemux *demux = CreateDemux();
    //视频解码器
    IDecode *vDecode = CreateDecode();
    //音频解码器
    IDecode *aDecode = CreateDecode();
    //解码器观察解封装器
    demux->observable->AddObserver(vDecode->observer);
    demux->observable->AddObserver(aDecode->observer);
    //显示观察视频解码器
    IVideoView *view = CreateVideoView();
    vDecode->observable->AddObserver(view->observer);
    //重采样观察音频解码器
    IResample *resample = CreateResample();
    aDecode->observable->AddObserver(resample->resampleObserver);
    //音频播放观察重采样
    IAudioPlay *audioPlay = CreateAudioPlay();
    resample->observable->AddObserver(audioPlay->audioObserver);

    player->demux = demux;          //解封装器
    player->vDecode = vDecode;      //视频解码器
    player->aDecode = aDecode;      //音频解码器
    player->videoView = view;       //视频播放器
    player->resample = resample;    //音频重采样
    player->audioPlay = audioPlay;  //音频播放器
    return player;
}