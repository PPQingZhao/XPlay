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

public class MainViewModel {

    private static final String TAG = MainViewModel.class.getSimpleName();
    private final Context mContext;
    private final UICallback mUICallback;
    private ObservableField<Boolean> obserPlaying = new ObservableField<>(false);

    public MainViewModel(Context context, UICallback uiCallback) {
        this.mContext = context;
        this.mUICallback = uiCallback;
    }

    public void startPlay() {
        if (obserPlaying.get()) {
            Toast.makeText(mContext, "playing", Toast.LENGTH_SHORT).show();
            return;
        }
        obserPlaying.set(true);
        String sdcard_path = Environment.getExternalStorageDirectory().getAbsolutePath();
        Jni.open(sdcard_path + "/ffmpeg/VID_20181015_164136.mp4");
//        Jni.open("/mnt/sdcard/VID_20180214_164359.mp4");
    }

    public void onStartPlay() {
        Log.e(TAG, "=======>>onStartPlay ");
        if (!PermissionUtils.isGranted(Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
            if (null != mUICallback) {
                mUICallback.requestPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE);
            }
        } else {
            startPlay();
        }
    }

    public void onStopPlay() {
        Log.e(TAG, "=======>>onStop ");
    }
}
