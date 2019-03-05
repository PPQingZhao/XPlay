//
// Created by qing on 19-1-5.
//

#include "IVideoView.h"
#include "../log/XLog.h"

class VideoViewObserver : public IObserver {
public:
    void Update(XData d) {
//        XLOGE("=VideoViewObserver==========Update: %d", d.decodeType);
        this->videoView->Render(d);
    };
    IVideoView *videoView = 0;

    VideoViewObserver(IVideoView *videoView) {
        this->videoView = videoView;
    };
};

IVideoView::IVideoView() {
    observer = new VideoViewObserver(this);
}