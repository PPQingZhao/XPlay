package com.pp.client.xplay.manager;


import com.pp.client.xplay.jni.Jni;

import java.util.Timer;
import java.util.TimerTask;

public class PlayManager {

    private long preview_id = -1;
    private Timer mTimer;
    private TimerTask mTimerTask;

    public long getPreview_id() {
        return preview_id;
    }

    public void startPlay(long surface_id, String url, boolean isHardDecode) {
        close();
        if (-1 == surface_id) return;
        Jni.open(surface_id, url, isHardDecode);
        preview_id = surface_id;
    }

    public void close() {
        stopTimer();
        close(preview_id);
        preview_id = -1;
    }

    public static void close(long preview_id) {
        if (-1 == preview_id) return;
        Jni.close(preview_id);
    }

    public double getCurrPos() {
        return Jni.getPlayPos(preview_id);
    }

    public void getPlayPos(final PlayCallback playCallback) {
        stopTimer();
        mTimer = new Timer();
        mTimerTask = new TimerTask() {
            @Override
            public void run() {
                double currPos = getCurrPos();
                if (null != playCallback) {
                    playCallback.playPosCallback(currPos);
                }
            }
        };
        mTimer.schedule(mTimerTask, 0, 500);
    }

    private void stopTimer() {
        if (null != mTimer) {
            mTimer.cancel();
            mTimer = null;
        }
        if (null != mTimerTask) {
            mTimerTask.cancel();
            mTimerTask = null;
        }
    }

    public void setPause(boolean pause) {
        Jni.setPause(preview_id, pause);
    }

    public boolean isPausing() {
        return Jni.isPausing(preview_id);
    }

    public void seek(double pos) {
        Jni.seek(preview_id, pos);
    }

    public interface PlayCallback {
        void playPosCallback(double pos);
    }
}
