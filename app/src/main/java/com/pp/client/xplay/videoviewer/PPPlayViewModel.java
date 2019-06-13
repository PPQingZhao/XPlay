package com.pp.client.xplay.videoviewer;

import android.content.Context;
import android.databinding.ObservableField;
import android.os.Environment;
import android.text.TextUtils;
import android.util.Log;
import android.widget.SeekBar;

import com.pp.client.xplay.manager.PlayManager;
import com.pp.client.xplay.videoviewer.callback.VideoViewerHelper;

public class PPPlayViewModel {

    private final Context mContext;
    private final PlayManager mPlayManager;
    private final VideoViewerHelper mViewerHelper;
    private String url;
    public final ObservableField<Integer> obserProgress = new ObservableField<>(0);
    public final ObservableField<Integer> obserSeekBarMax = new ObservableField<>(1000);
    public final ObservableField<Boolean> obserPause = new ObservableField<>(true);
    ObservableField<Boolean> obserSeekTouch = new ObservableField<>(false);

    public PPPlayViewModel(Context context, VideoViewerHelper viewerHelper) {
        this.mContext = context;
        this.mViewerHelper = viewerHelper;
        mPlayManager = new PlayManager();
        String sdcard_path = Environment.getExternalStorageDirectory().getAbsolutePath();
        /*
        RTMP协议直播源
        香港卫视：rtmp://live.hkstv.hk.lxdns.com/live/hks
        芜湖综合频道 : http://live.wuhubtv.com/channels/1531/500.flv/live
        蚌埠生活频道 : rtmp://vod.ahbbtv.com:1935/live/shpd1
        淮北新闻频道 : rtmp://vod.0561rtv.com:1935/live/live1
        淮北影视频道 : rtmp://vod.0561rtv.com:1935/live/live4
        重庆卫视 ：rtmp://live5.cqnews.net:1935/live/TVFLV15

        RTSP协议直播源
        珠海过澳门大厅摄像头监控：rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp
        大熊兔（点播）：rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov

        HTTP协议直播源
        香港卫视：http://live.hkstv.hk.lxdns.com/live/hks/playlist.m3u8
        CCTV1高清：http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8
        CCTV3高清：http://ivi.bupt.edu.cn/hls/cctv3hd.m3u8
        CCTV5高清：http://ivi.bupt.edu.cn/hls/cctv5hd.m3u8
        CCTV5+高清：http://ivi.bupt.edu.cn/hls/cctv5phd.m3u8
        CCTV6高清：http://ivi.bupt.edu.cn/hls/cctv6hd.m3u8
       */
//        url = "http://live.hkstv.hk.lxdns.com/live/hks/playlist.m3u8";
//        url = "http://live.wuhubtv.com/channels/1531/500.flv/live";
//        url = "rtmp://vod.ahbbtv.com:1935/live/shpd1";
//        url = "rtmp://vod.0561rtv.com:1935/live/live1";
//        url = "rtmp://vod.0561rtv.com:1935/live/live4";
//        url = "rtmp://live5.cqnews.net:1935/live/TVFLV15";
//        url = "rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp";
//        url = "rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov"; //有图
//        url = "http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8";   //cctv 1
//        url = "http://ivi.bupt.edu.cn/hls/cctv3hd.m3u8";   //　cctv3
//        url = "http://ivi.bupt.edu.cn/hls/cctv5hd.m3u8";
    }

    public void startPlay(final String url) {
        if (TextUtils.isEmpty(url)) return;
        if (null == mViewerHelper) return;
        mViewerHelper.startRender(new XPlay.SurfaceHolderCallback() {
            @Override
            public void surfaceCreated(long surface_id) {
                mPlayManager.startPlay(surface_id, url, false);
                mPlayManager.getPlayPos(playCallback);
                obserPause.set(false);
            }
        });
    }

    public void seek(double pos) {
        if (pos < 0 || pos > 1) return;
        mPlayManager.seek(pos);
    }

    SeekBar.OnSeekBarChangeListener onSeekBarChangeListener = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
            obserSeekTouch.set(true);
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            obserSeekTouch.set(false);
            double pos = (double) seekBar.getProgress() / (double) seekBar.getMax();
            Log.e("TAG", "**************** onStopTrackingTouch pos: " + pos);
            seek(pos);
        }
    };

    PlayManager.PlayCallback playCallback = new PlayManager.PlayCallback() {
        @Override
        public void playPosCallback(double pos) {
            if (!obserSeekTouch.get()) {
                obserProgress.set((int) (obserSeekBarMax.get() * pos));
            }
        }
    };

    public void stopPlay() {
        mPlayManager.close();
        obserPause.set(true);
    }

    /**********************点击事件 start**************************/
    public void onPlayOrPause() {
        boolean pausing = mPlayManager.isPausing();
        Log.e("TAG", "pausing: 　" + pausing);
        mPlayManager.setPause(!pausing);
        obserPause.set(!pausing);
    }

    /**********************点击事件 end**************************/
}
