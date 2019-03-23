//
// Created by qing on 19-3-6.
//

#ifndef XPLAY_IPLAYERPROXY_H
#define XPLAY_IPLAYERPROXY_H


#include <mutex>
#include "../player/IPlayer.h"
#include <map>

class IPlayerProxy : public IPlayer {
public:
    static IPlayerProxy *Get() {
        static IPlayerProxy playerProxy;
        return &playerProxy;
    }
    void InitHard(void *vm);
    void Init(long key);

    virtual bool Open(const char *path);

    virtual bool Start();

    virtual bool InitView(void *win);

protected:
    IPlayer *player = 0;
    std::mutex mutex;
    std::map<long, IPlayer *> playMap;
};


#endif //XPLAY_IPLAYERPROXY_H
