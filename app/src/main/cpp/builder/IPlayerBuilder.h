//
// Created by qing on 19-3-5.
//

#ifndef XPLAY_IPLAYERBUILDER_H
#define XPLAY_IPLAYERBUILDER_H

#include "../player/IPlayer.h"

class IPlayerBuilder {
public:
    virtual IPlayer *BuilderPlayer(unsigned char index = 0);

protected:
    virtual IDemux *CreateDemux() = 0;

    virtual IDecode *CreateDecode()=0;

    virtual IResample *CreateResample() =0;

    virtual IVideoView *CreateVideoView() =0;

    virtual IAudioPlay *CreateAudioPlay() = 0;

    virtual IPlayer *CreateIplayer(unsigned char index =0)= 0;
};


#endif //XPLAY_IPLAYERBUILDER_H
