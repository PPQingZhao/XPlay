package com.pp.client.xplay.jni;

public class Jni {
    static {
        System.loadLibrary("native-lib");
    }

    public native static String stringFromJNI();

    public native static void open(String url);

    public native static void initOpenGl(Object holder);
}
