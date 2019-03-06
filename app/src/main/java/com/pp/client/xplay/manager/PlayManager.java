package com.pp.client.xplay.manager;


import com.pp.client.xplay.jni.Jni;

public class PlayManager {

    private long preview_id = -1;


    public long getPreview_id() {
        return preview_id;
    }

    public void startPlay(long surface_id, String url) {
        if (-1 == surface_id) return;
        Jni.open(surface_id,url);
    }
}
