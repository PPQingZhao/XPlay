package com.pp.client.xplay.util;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import java.lang.ref.WeakReference;

public abstract class WeakReferenceHandler<T> extends Handler {
    private WeakReference<T> mReference;

    public WeakReferenceHandler(T reference) {
        super();
        mReference = new WeakReference<T>(reference);
    }

    public WeakReferenceHandler(T reference, Callback callback) {
        super(callback);
        mReference = new WeakReference<T>(reference);
    }

    public WeakReferenceHandler(T reference, Looper looper) {
        super(looper);
        mReference = new WeakReference<T>(reference);
    }

    public WeakReferenceHandler(T reference, Looper looper, Callback callback) {
        super(looper, callback);
        mReference = new WeakReference<T>(reference);
    }

    @Override
    public void handleMessage(Message msg) {
        if (mReference.get() == null) {
            return;
        }
        handleMessage(mReference.get(), msg);
    }

    protected abstract void handleMessage(T reference, Message msg);
}
