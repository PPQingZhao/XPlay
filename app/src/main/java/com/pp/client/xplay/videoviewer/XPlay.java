package com.pp.client.xplay.videoviewer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;

import com.pp.client.xplay.jni.Jni;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback {

    private static final String TAG = XPlay.class.getSimpleName();
    private GLRenderer mGLRender;

    public long getSurface_id() {
        return surface_id;
    }

    private long surface_id = -1;

    public XPlay(Context context) {
        this(context, null);
    }

    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
        mGLRender = new GLRenderer();
        setRenderer(mGLRender);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.e(TAG, "=============>>11  surfaceCreated");
        Log.e(TAG, "=============>>getSurface: " + holder.getSurface());
        //初始化opengl
        surface_id = Jni.initOpenGl(holder.getSurface());
        if (null != surfaceHolderCallback) {
            surfaceHolderCallback.surfaceCreated(surface_id);
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }

    private String SDCARDPATH = Environment.getExternalStorageDirectory().getAbsolutePath();

    class GLRenderer implements Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
        }

        @Override
        public void onDrawFrame(GL10 gl) {

        }
    }

    private SurfaceHolderCallback surfaceHolderCallback;

    public void setSurfaceHolderCallback(SurfaceHolderCallback surfaceHolderCallback) {
        this.surfaceHolderCallback = surfaceHolderCallback;
    }

    interface SurfaceHolderCallback {
        public void surfaceCreated(long surface_id);
    }
}
