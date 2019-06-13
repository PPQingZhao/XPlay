

#include "FFPlayerBuilder.h"
#include "../demux/FFDemux.h"
#include "../decode/FFDecode.h"
#include "../audio/FFResample.h"
#include "../videoviewer/GLVideoView.h"
#include "../audio/SLAudioPlay.h"

void FFPlayerBuilder::InitHard(void *vm) {
    FFDecode::InitHard(vm);
}

IDemux *FFPlayerBuilder::CreateDemux() {
    IDemux *demux = new FFDemux();
    return demux;
}

IDecode *FFPlayerBuilder::CreateDecode() {
    IDecode *decode = new FFDecode();
    return decode;
}

IResample *FFPlayerBuilder::CreateResample() {
    IResample *resample = new FFResample();
    return resample;
}

IVideoView *FFPlayerBuilder::CreateVideoView() {
    IVideoView *videoView = new GLVideoView();
    return videoView;
}

IAudioPlay *FFPlayerBuilder::CreateAudioPlay() {
    IAudioPlay *audioPlay = new SLAudioPlay();
    return audioPlay;
}

IPlayer *FFPlayerBuilder::CreateIplayer() {
    return IPlayer::Get();
}