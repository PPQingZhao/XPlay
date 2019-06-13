package com.pp.client.xplay.videoviewer;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;

import com.pp.client.xplay.jni.Jni;
import com.pp.client.xplay.manager.PlayManager;

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
//        setEGLContextClientVersion(2);
        mGLRender = new GLRenderer();
        setRenderer(mGLRender);
//        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
//        super.surfaceCreated(holder);
        Log.e(TAG, "=============>>11  surfaceCreated");
        if (surface_id != -1) {
            PlayManager.close(surface_id);
        }
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

    public void clear() {
        surface_id = -1;
    }

    class GLRenderer implements Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
//            GLES20.glViewport(0, 0, width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
//            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
//            GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }

    private SurfaceHolderCallback surfaceHolderCallback;

    public void setSurfaceHolderCallback(SurfaceHolderCallback surfaceHolderCallback) {
        this.surfaceHolderCallback = surfaceHolderCallback;
    }

    public interface SurfaceHolderCallback {
        public void surfaceCreated(long surface_id);
    }
}
