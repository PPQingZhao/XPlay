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

    long BuilderPlayer();

    virtual bool Open(long key, const char *path, bool isHardDecode);

    virtual void Close(long key);

    virtual bool Start(long key);

    virtual bool Seek(long key, double pos);

    virtual bool InitView(long key, void *win);

    double PlayPos(long key);

    bool IsPausing(long key);

    void SetPause(long key, bool isP);

protected:
    std::mutex mutex;
    std::map<long, IPlayer *> playerMap;
};


#endif //XPLAY_IPLAYERPROXY_H
