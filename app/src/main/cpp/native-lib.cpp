#include "demux/FFDemux.h"
#include "log/XLog.h"
#include "decode/FFDecode.h"
#include "decode/IDecode.h"
#include "native-lib.h"
#include "videoviewer/XEGL.h"
#include "videoviewer/XShader.h"
#include "videoviewer/IVideoView.h"
#include "videoviewer/GLVideoView.h"
#include "audio/FFResample.h"
#include "audio/SLAudioPlay.h"
#include "player/IPlayer.h"
#include <android/native_window_jni.h>

JNI_JINT_RETURN JNI_OnLoad(JavaVM *vm, void *res) {
    JNIEnv *venv;
    if (vm->GetEnv((void **) &venv, JNI_VERSION_1_6) != JNI_OK) {
        XLOGE("============>> JNI_OnLoad  failed!");
        return -1;
    }
    FFDecode::InitHard(vm);
    XLOGE("==============>> JNI_OnLoad");
    return JNI_VERSION_1_6;
}
JNI_JSTRING_RETURN JNI_FUNCTION(stringFromJNI)(JNI_DEFAULT_PARAM) {
    std::string hello = "Hello from C++";
    IDemux *de = new FFDemux();
    de->Open("");
    return env->NewStringUTF(hello.c_str());
}
IVideoView *view = NULL;
JNI_VOID_RETURN JNI_FUNCTION(open)(JNI_DEFAULT_PARAM, jstring url_) {
    if (view == NULL) {
        XLOGE("Open failed because view is NULL!");
        return;
    }
    const char *url = env->GetStringUTFChars(url_, 0);
    XLOGE("==============>> url: %s ", url);
    XLOGE("open");
    //测试代码
    //解封装器
    IDemux *demux = new FFDemux();
    //打开解封装器
    bool success = demux->Open(url);
    //视频解码器
    IDecode *videoDecode = new FFDecode();
    videoDecode->Open(demux->GetVideoParameter(), DECODE_HARD);
    //音频解码器
    IDecode *audioDecode = new FFDecode();
    audioDecode->Open(demux->GetAudioParameter());
    //添加观察者
    demux->observable->AddObserver(videoDecode->observer);
//    demux->observable->AddObserver(audioDecode->observer);

    videoDecode->observable->AddObserver(view->observer);

    IResample *resample = new FFResample();
    XParameter out = demux->GetAudioParameter();
    resample->Open(demux->GetAudioParameter(), out);
    audioDecode->observable->AddObserver(resample->resampleObserver);

    //音频播放器
    SLAudioPlay *audioPlay = new SLAudioPlay();
    audioPlay->StartPlay(out);
    resample->observable->AddObserver(audioPlay->audioObserver);
    //打开解码器
    videoDecode->Open(demux->GetVideoParameter(),DECODE_SOFT);
//    audioDecode->Open(demux->GetAudioParameter());


    IPlayer::Get(0)->Open(url);
    //启动解封装线程
    demux->Start();
    //启动解码线程
    videoDecode->Start();
//    audioDecode->Start();

    env->ReleaseStringUTFChars(url_, url);
}

JNI_VOID_RETURN JNI_FUNCTION(initOpenGl)(JNI_DEFAULT_PARAM, jobject surface) {
    XLOGE("initOpenGl");
    //创建窗口
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (NULL != view) {
        XLOGE("IVideoView was alread init when call initOpenGl!");
        return;
    }
    view = new GLVideoView();
    view->SetRender(nativeWindow);
}