package com.pp.client.xplay.videoviewer;

import android.content.Context;
import android.support.v4.util.Pools;
import android.util.Log;

/**
 * Created by qing on 18-10-16.
 */

public class XplayPool {
    public static final Pools.SynchronizedPool<XPlay> synchronizedPool = new Pools.SynchronizedPool<>(3);

    public static XPlay obtain(Context context) {
        XPlay t = synchronizedPool.acquire();
        Log.e("XplayPool", "=======>> " + t);
        return t != null ? t : new XPlay(context);
    }

    public static void release(XPlay xPlay) {
        synchronizedPool.release(xPlay);
    }
}
