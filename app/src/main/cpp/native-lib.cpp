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
#include "builder/FFPlayerBuilder.h"
#include <android/native_window_jni.h>
#include <map>

std::map<long, IPlayer *> playMap;
JNI_JINT_RETURN JNI_OnLoad(JavaVM *vm, void *res) {
    JNIEnv *venv;
    if (vm->GetEnv((void **) &venv, JNI_VERSION_1_6) != JNI_OK) {
        XLOGE("============>> JNI_OnLoad  failed!");
        return JNI_ERR;
    }
    jni_global_info.vm = vm;
    FFPlayerBuilder::InitHard(vm);
    return JNI_VERSION_1_6;
}

JNI_VOID_RETURN JNI_FUNCTION(open)(JNI_DEFAULT_PARAM, jlong surface_id, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    XLOGE("==============>> url: %s ", url);
    XLOGE("==============>> size: %d ", playMap.size());
    playMap[surface_id]->Open(url);
    playMap[surface_id]->Start();
    env->ReleaseStringUTFChars(url_, url);
}

JNI_JLONG_RETURN JNI_FUNCTION(initOpenGl)(JNI_DEFAULT_PARAM, jobject surface) {
    XLOGE("initOpenGl");
    IPlayer *player = FFPlayerBuilder::Get()->BuilderPlayer();
    /* std::map<long, IPlayer *>::iterator itFind = playMap.find((long) surface);
     XLOGE("*************** surface: %p" ,surface);
     if (itFind == playMap.end()) {
         XLOGE("*************** insert");
     }*/
    playMap.insert(std::make_pair((long) player, player));
    //创建窗口
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    player->InitView(nativeWindow);
    return (jlong) player;
}