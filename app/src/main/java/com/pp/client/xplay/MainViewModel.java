package com.pp.client.xplay;

import android.Manifest;
import android.content.Context;
import android.databinding.ObservableField;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

import com.blankj.utilcode.util.PermissionUtils;
import com.pp.client.xplay.callback.UICallback;
import com.pp.client.xplay.jni.Jni;
import com.pp.client.xplay.videoviewer.PPPlayViewModel;

import java.util.HashSet;
import java.util.Set;

public class MainViewModel {

    private static final String TAG = MainViewModel.class.getSimpleName();
    private final Context mContext;
    private final UICallback mUICallback;
    private ObservableField<Boolean> obserPlaying = new ObservableField<>(false);
    public ObservableField<String> obserFFmpegInfo = new ObservableField<>("");

    public MainViewModel(Context context, UICallback uiCallback) {
        this.mContext = context;
        this.mUICallback = uiCallback;
        init();
    }

    private void init() {
        String fFmpegInfo = Jni.getFFmpegInfo();
        obserFFmpegInfo.set(null == fFmpegInfo ? "" : fFmpegInfo);
    }

    public void onStop() {
        stop();
    }

    public void startPlay() {
        if (obserPlaying.get()) {
            Toast.makeText(mContext, "playing", Toast.LENGTH_SHORT).show();
            return;
        }
        String sdcard_path = Environment.getExternalStorageDirectory().getAbsolutePath();
//        String url = sdcard_path + "/ffmpeg/1080.mp4";
//        String url = "rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov";
//        String url = "http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8";
//        String url = "http://ivi.bupt.edu.cn/hls/cctv3hd.m3u8";
//
//        香港卫视：rtmp://live.hkstv.hk.lxdns.com/live/hks
//        芜湖综合频道 : http://live.wuhubtv.com/channels/1531/500.flv/live
//        蚌埠生活频道 : rtmp://vod.ahbbtv.com:1935/live/shpd1
//        淮北新闻频道 : rtmp://vod.0561rtv.com:1935/live/live1
//        淮北影视频道 : rtmp://vod.0561rtv.com:1935/live/live4
//        重庆卫视 ：rtmp://live5.cqnews.net:1935/live/TVFLV15
//        String url = "rtmp://live5.cqnews.net:1935/live/TVFLV15";
//        String url = "rtmp://vod.0561rtv.com:1935/live/live4";
//        String url = "rtmp://vod.ahbbtv.com:1935/live/shpd1";
//        String url = "rtmp://live.hkstv.hk.lxdns.com/live/hks";
//        String url = "rtmp://116.213.200.53/tslsChannelLive/PCG0DuD/live";
//        String url = "http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8";
        String url = "rtmp://live.hkstv.hk.lxdns.com/live/hks2";
        obserPlaying.set(true);
        for (PPPlayViewModel viewModel :
                videoModels) {
            if (null == viewModel)
                continue;
            viewModel.startPlay(url);
            break;
        }
    }

    private void stop() {
        for (PPPlayViewModel viewModel : videoModels) {
            if (null == viewModel)
                continue;
            viewModel.stopPlay();
        }
        obserPlaying.set(false);
    }

    public void onStartPlay() {
        Log.e(TAG, "=======>>onStartPlay ");
        if (!PermissionUtils.isGranted(Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.INTERNET)) {
            if (null != mUICallback) {
                mUICallback.requestPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE);
            }
        } else {
            startPlay();
        }
    }

    public void onStopPlay() {
        stop();
    }

    Set<PPPlayViewModel> videoModels = new HashSet<PPPlayViewModel>();

    public void addVideoModel(PPPlayViewModel viewModel) {
        videoModels.add(viewModel);
    }
}
