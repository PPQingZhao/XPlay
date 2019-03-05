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

/**
 * Created by qing on 18-10-16.
 */

public class XPlay extends GLSurfaceView implements Runnable,SurfaceHolder.Callback{

    private static final String TAG = XPlay.class.getSimpleName();
    private GLRenderer mGLRender;

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
        Log.e(TAG,"=============>>11  surfaceCreated");
        //初始化opengl
        Jni.initOpenGl(holder.getSurface());
        new Thread(XPlay.this).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }

    @Override
    public void run() {
        Log.e(TAG, "===============>> getHolder().getSurface(): " +getHolder().getSurface());
    }

    private String SDCARDPATH = Environment.getExternalStorageDirectory().getAbsolutePath();

    class GLRenderer implements Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Log.e(TAG,"=============>>  onSurfaceCreated");
            //初始化opengl
//            Jni.initOpenGl(getHolder().getSurface());
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
        }

        @Override
        public void onDrawFrame(GL10 gl) {

        }
    }
}
