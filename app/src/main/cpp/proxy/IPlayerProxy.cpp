//
// Created by qing on 19-3-6.
//

#include "IPlayerProxy.h"
#include "../builder/FFPlayerBuilder.h"

void IPlayerProxy::InitHard(void *vm) {
    mutex.lock();
    if (vm) {
        FFPlayerBuilder::InitHard(vm);
    }
    mutex.unlock();
}

void IPlayerProxy::Init(long key) {
    mutex.lock();
    if (!player) {
        player = FFPlayerBuilder::Get()->BuilderPlayer();
        playMap.insert(std::make_pair(key, player));
    }
    mutex.unlock();
}

bool IPlayerProxy::Open(const char *path) {
    bool ret = false;
    mutex.lock();
    if (player) {
        ret = player->Open(path);
    }
    mutex.unlock();
    return ret;
}

bool IPlayerProxy::Start() {
    bool ret = false;
    mutex.lock();
    if (player) {
        ret = player->Start();
    }
    mutex.unlock();
    return ret;
}

bool IPlayerProxy::InitView(void *win) {
    bool ret = false;
    mutex.lock();
    if (player) {
        ret = player->InitView(win);
    }
    mutex.unlock();
    return ret;
}