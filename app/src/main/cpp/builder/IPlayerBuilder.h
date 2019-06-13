//
// Created by qing on 19-3-5.
//

#ifndef XPLAY_IPLAYERBUILDER_H
#define XPLAY_IPLAYERBUILDER_H

#include <map>
#include "../player/IPlayer.h"

class IPlayerBuilder {
public:
    virtual IPlayer *BuilderPlayer();

protected:
    virtual IDemux *CreateDemux() = 0;

    virtual IDecode *CreateDecode()=0;

    virtual IResample *CreateResample() =0;

    virtual IVideoView *CreateVideoView() =0;

    virtual IAudioPlay *CreateAudioPlay() = 0;

    virtual IPlayer *CreateIplayer()= 0;

};


#endif //XPLAY_IPLAYERBUILDER_H
