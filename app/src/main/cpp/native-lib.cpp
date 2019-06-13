
extern "C" {
#include <libavcodec/avcodec.h>
}

#include "demux/FFDemux.h"
#include "log/XLog.h"
#include "decode/FFDecode.h"
#include "decode/IDecode.h"
#include "native-lib.h"
#include "audio/FFResample.h"
#include "audio/SLAudioPlay.h"
#include "player/IPlayer.h"
#include "builder/FFPlayerBuilder.h"
#include "proxy/IPlayerProxy.h"
#include <android/native_window_jni.h>
#include <map>

JNI_JINT_RETURN JNI_OnLoad(JavaVM *vm, void *res) {
    JNIEnv *venv;
    if (vm->GetEnv((void **) &venv, JNI_VERSION_1_6) != JNI_OK) {
        XLOGE("============>> JNI_OnLoad  failed!");
        return JNI_ERR;
    }
    jni_global_info.vm = vm;
    IPlayerProxy::Get()->InitHard(vm);
    return JNI_VERSION_1_6;
}

JNI_JLONG_RETURN JNI_FUNCTION(open)(JNI_DEFAULT_PARAM, jlong surface_id, jstring url_,jboolean isHardDecode) {
    const char *url = env->GetStringUTFChars(url_, 0);
    XLOGE("==============>> url: %s ", url);
    IPlayerProxy::Get()->Open(surface_id, url, isHardDecode);
    IPlayerProxy::Get()->Start(surface_id);
    env->ReleaseStringUTFChars(url_, url);
    return surface_id;
}

JNI_VOID_RETURN JNI_FUNCTION(close)(JNI_DEFAULT_PARAM, jlong surface_id) {
    XLOGE("==============>> close()");
    IPlayerProxy::Get()->Close(surface_id);
}

JNI_JLONG_RETURN JNI_FUNCTION(initOpenGl)(JNI_DEFAULT_PARAM, jobject surface) {
    XLOGE("initOpenGl");
    //创建窗口
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    long key = IPlayerProxy::Get()->BuilderPlayer();
    IPlayerProxy::Get()->InitView(key, nativeWindow);
    return key;
}

JNI_JSTRING_RETURN JNI_FUNCTION(getFFmpegInfo)(JNI_DEFAULT_PARAM) {
    std::string info = "";
    info += avcodec_configuration();
    return env->NewStringUTF(info.c_str());
}

JNI_JDOUBLE_RETURN JNI_FUNCTION(getPlayPos)(JNI_DEFAULT_PARAM, jlong surface_id) {
//    XLOGE("getPlayPos");
    return IPlayerProxy::Get()->PlayPos(surface_id);
}

JNI_JBOOLEAN_RETURN JNI_FUNCTION(seek)(JNI_DEFAULT_PARAM, jlong surface_id, jdouble pos) {
    XLOGE("seek");
    return IPlayerProxy::Get()->Seek(surface_id, pos);
}

JNI_VOID_RETURN JNI_FUNCTION(setPause)(JNI_DEFAULT_PARAM, jlong surface_id, jboolean pause) {
    XLOGE("setPause");
    IPlayerProxy::Get()->SetPause(surface_id, pause);
}
JNI_JBOOLEAN_RETURN JNI_FUNCTION(isPausing)(JNI_DEFAULT_PARAM, jlong surface_id) {
    XLOGE("isPausing");
    return IPlayerProxy::Get()->IsPausing(surface_id);
}