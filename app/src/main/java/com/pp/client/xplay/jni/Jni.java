package com.pp.client.xplay.jni;

public class Jni {
    static {
        System.loadLibrary("native-lib");
    }


    public native static void open(long surface_id, String url);

    public native static long initOpenGl(Object holder);

    public native static String getFFmpegInfo();
}
