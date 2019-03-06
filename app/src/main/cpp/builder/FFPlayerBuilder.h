//
// Created by qing on 19-3-5.
//

#ifndef XPLAY_FFPLAYERBUILDER_H
#define XPLAY_FFPLAYERBUILDER_H


#include "IPlayerBuilder.h"

class FFPlayerBuilder : public IPlayerBuilder {
public:
    static void InitHard(void *vm);
    static FFPlayerBuilder *Get(){
        static FFPlayerBuilder ffPlayerBuilder;
        return &ffPlayerBuilder;
    }
protected:
    virtual IDemux *CreateDemux();

    virtual IDecode *CreateDecode();

    virtual IResample *CreateResample();

    virtual IVideoView *CreateVideoView();

    virtual IAudioPlay *CreateAudioPlay();

    virtual IPlayer *CreateIplayer(unsigned char index = 0);
};


#endif //XPLAY_FFPLAYERBUILDER_H
