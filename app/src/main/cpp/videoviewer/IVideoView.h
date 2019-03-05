//
// Created by qing on 19-1-5.
//

#ifndef XPLAY_IVIDEOVIEW_H
#define XPLAY_IVIDEOVIEW_H


#include "../data/XData.h"
#include "../observer/IObserver.h"

class IVideoView {
public:
    virtual void SetRender(void *win) = 0;

    virtual void Render(XData data) = 0;

    IObserver *observer;

    IVideoView();
};


#endif //XPLAY_IVIDEOVIEW_H
