package com.pp.client.xplay.videoviewer;

import android.opengl.GLSurfaceView;
import android.os.Environment;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by qing on 18-10-16.
 */

public class GLRenderer implements GLSurfaceView.Renderer{
    private String SDCARDPATH = Environment.getExternalStorageDirectory().getAbsolutePath();
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
