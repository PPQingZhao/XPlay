
#include "IPlayerProxy.h"
#include "../builder/FFPlayerBuilder.h"
#include "../log/XLog.h"

void IPlayerProxy::InitHard(void *vm) {
    mutex.lock();
    if (vm) {
        FFPlayerBuilder::InitHard(vm);
    }
    mutex.unlock();
}

long IPlayerProxy::BuilderPlayer() {
    mutex.lock();
    IPlayer *player = FFPlayerBuilder::Get()->BuilderPlayer();
    long key = -1;
    if (player) {
        XLOGE("IPlayerProxy::BuilderPlayer()  key: %d ", player);
        key = (long) player;
        playerMap.insert(std::make_pair(key, player));
    }
    mutex.unlock();
    return key;
}

void IPlayerProxy::Close(long key) {
    mutex.lock();
    if (playerMap[key]) {
        playerMap[key]->Close();
    }
    mutex.unlock();
}

double IPlayerProxy::PlayPos(long key) {
    mutex.lock();
    double pos = 0.0;
    if (playerMap[key]) {
        pos = playerMap[key]->PlayPos();
    }
    mutex.unlock();
    return pos;
}

bool IPlayerProxy::Open(long key, const char *path, bool hardDecode) {
    bool ret = false;
    mutex.lock();
    if (playerMap[key]) {
        XLOGE("IPlayerProxy::Open() start!");
        playerMap[key]->isHardDecode = hardDecode;
        ret = playerMap[key]->Open(path);
    }
    mutex.unlock();
    return ret;
}

bool IPlayerProxy::Start(long key) {
    bool ret = false;
    mutex.lock();
    if (playerMap[key]) {
        ret = playerMap[key]->Start();
    }
    mutex.unlock();
    return ret;
}

bool IPlayerProxy::Seek(long key, double pos) {
    double ret = false;
    mutex.lock();
    if (playerMap[key]) {
        ret = playerMap[key]->Seek(pos);
    }
    mutex.unlock();
    return ret;
}

bool IPlayerProxy::IsPausing(long key) {
    bool ret = false;
    mutex.lock();
    if (playerMap[key]) {
        ret = playerMap[key]->IsPausing();
    }
    mutex.unlock();
    return ret;
}

void IPlayerProxy::SetPause(long key, bool isP) {
    mutex.lock();
    if (playerMap[key]) {
        playerMap[key]->SetPause(isP);
    }
    mutex.unlock();
}

bool IPlayerProxy::InitView(long key, void *win) {
    bool ret = false;
    mutex.lock();
    if (playerMap[key]) {
        ret = playerMap[key]->InitView(win);
    }
    mutex.unlock();
    return ret;
}