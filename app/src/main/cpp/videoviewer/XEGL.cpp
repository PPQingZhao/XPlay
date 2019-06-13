//
// Created by qing on 19-1-3.
//

#include <android/native_window.h>
#include <EGL/egl.h>
#include <mutex>
#include "XEGL.h"
#include "../log/XLog.h"

class CXEGL : public XEGL {
public:
    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    EGLSurface eglSurface = EGL_NO_SURFACE;
    EGLContext eglContext = EGL_NO_CONTEXT;
    std::mutex mux;

    virtual void Draw() {
//        XLOGE("XEGL Draw!");
        mux.lock();
        if (eglDisplay == EGL_NO_DISPLAY || eglSurface == EGL_NO_SURFACE) {
            XLOGE("EGL init falied!");
            mux.unlock();
            return;
        }
        eglSwapBuffers(eglDisplay, eglSurface);
        mux.unlock();
    }

    virtual void Close() {
        mux.lock();
        if (eglDisplay == EGL_NO_DISPLAY) {
            mux.unlock();
            return;
        }
        eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (eglSurface != EGL_NO_SURFACE) {
            eglDestroySurface(eglDisplay, eglSurface);
        }
        if (eglContext != EGL_NO_CONTEXT) {
            eglDestroyContext(eglDisplay, eglContext);
        }
        eglTerminate(eglDisplay);
        eglDisplay = EGL_NO_DISPLAY;
        eglSurface = EGL_NO_SURFACE;
        eglContext = EGL_NO_CONTEXT;
        mux.unlock();
    }

    virtual bool Init(void *win) {
        Close();
        ANativeWindow *nativeWindow = (ANativeWindow *) win;
        //初始化EGL
        //①获取EGLDisplay对象　显示设备
        mux.lock();
        this->eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (eglDisplay == EGL_NO_DISPLAY) {
            XLOGE("eglGetDisplay failed!");
            mux.unlock();
            return false;
        }
        //②初始化disPlay
        if (EGL_TRUE != eglInitialize(eglDisplay, 0, 0)) {
            XLOGE("eglInitialize failed!");
            mux.unlock();
            return false;
        }
        //③获取配置并创建surface
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfigs = 0;
        if (EGL_TRUE != eglChooseConfig(eglDisplay, configSpec, &config, 1, &numConfigs)) {
            XLOGE("eglChooseConfig failed!");
            mux.unlock();
            return false;
        }
        //创建surface
        eglSurface = eglCreateWindowSurface(eglDisplay, config, nativeWindow, NULL);
        if (eglSurface == EGL_NO_SURFACE) {
            XLOGE("eglCreateWindowSurface failed!");
            mux.unlock();
            return false;
        }
        //④创建并打开上下文
        const EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, ctxAttr);
        if (eglContext == EGL_NO_CONTEXT) {
            XLOGE("eglCreateContext failed!");
            mux.unlock();
            return false;
        }
        //上下文切换
        if (EGL_TRUE != eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
            XLOGE("eglMakeCurrent faliled!");
            mux.unlock();
            return false;
        }
        XLOGE("ANativeWindow init success!");
        mux.unlock();
        return true;
    }
};

XEGL *XEGL::GetIntance() {
//    static CXEGL egl;
//    return &egl;
    return new CXEGL();
}
