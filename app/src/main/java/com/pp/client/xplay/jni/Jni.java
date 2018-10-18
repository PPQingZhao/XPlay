package com.pp.client.xplay.jni;

/**
 * Created by qing on 18-10-8.
 */

public class Jni {
    static {
        System.loadLibrary("native-lib");
    }
    public static native String stringFromJNI();

}
