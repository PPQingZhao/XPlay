package com.pp.client.xplay.videoviewer.callback;

import com.pp.client.xplay.videoviewer.XPlay;

public interface VideoViewerHelper {
    void startRender(XPlay.SurfaceHolderCallback holderCallback);
    void stopRender();
}
