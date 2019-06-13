package com.pp.client.xplay.jni;

public class Jni {
    static {
        System.loadLibrary("native-lib");
    }


    public native static long open(long surface_id, String url, boolean isHardDecode);

    public native static void close(long surface_id);

    public native static long initOpenGl(Object holder);

    public native static String getFFmpegInfo();

    public native static double getPlayPos(long surface_id);

    public native static boolean seek(long surface_id, double pos);

    public native static void setPause(long surface_id, boolean pause);

    public native static boolean isPausing(long surface_id);
}
