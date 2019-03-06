package com.pp.client.xplay.videoviewer;

import android.content.Context;
import android.os.Environment;
import android.text.TextUtils;
import android.util.Log;

import com.pp.client.xplay.manager.PlayManager;
import com.pp.client.xplay.videoviewer.callback.VideoViewerHelper;

public class PPPlayViewModel {

    private final Context mContext;
    private final PlayManager mPlayManager;
    private final VideoViewerHelper mViewerHelper;
    private String url;

    public PPPlayViewModel(Context context, VideoViewerHelper viewerHelper) {
        this.mContext = context;
        this.mViewerHelper = viewerHelper;
        mPlayManager = new PlayManager();
        String sdcard_path = Environment.getExternalStorageDirectory().getAbsolutePath();
        url = sdcard_path + "/ffmpeg/VID_20181015_164136.mp4";
//        url = "/mnt/sdcard/VID_20180214_164359.mp4";
//        Jni.open(sdcard_path + "/ffmpeg/VID_20181015_164136.mp4");
//        Jni.open("/mnt/sdcard/VID_20180214_164359.mp4");
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public void startPlay(String url) {
        if (TextUtils.isEmpty(url)) return;
        if (null == mViewerHelper) return;
        long surface_id = mViewerHelper.startRender();
        mPlayManager.startPlay(surface_id, url);
    }

    /**********************点击事件 start**************************/
    public void onPlay() {
        Log.e("TAG", "*********  click on onPlay()");
        startPlay(url);
    }
    /**********************点击事件 end**************************/
}
