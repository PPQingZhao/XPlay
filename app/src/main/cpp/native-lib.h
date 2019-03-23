//
// Created by qing on 19-1-3.
//

#ifndef XPLAY_NATIVE_LIB_H_H
#define XPLAY_NATIVE_LIB_H_H

#include <jni.h>
#include <string.h>

#define JNI_FUNCTION(NAME)      Java_com_pp_client_xplay_jni_Jni_##NAME
#define JNI_DEFAULT_PARAM       JNIEnv *env, jobject instance
#define JNI_VOID_RETURN         extern "C" JNIEXPORT void  JNICALL
#define JNI_JSTRING_RETURN      extern "C" JNIEXPORT jstring JNICALL
#define JNI_JINT_RETURN         extern "C" JNIEXPORT jint JNICALL
#define JNI_JLONG_RETURN        extern "C" JNIEXPORT jlong JNICALL

/**
 * 传入媒体文件路径,进行解码
 */
JNI_VOID_RETURN JNI_FUNCTION(open)(JNI_DEFAULT_PARAM, jlong surface_id, jstring url_);
/**
 * 初始化opengl,传入窗口句柄
 */
JNI_JLONG_RETURN JNI_FUNCTION(initOpenGl)(JNI_DEFAULT_PARAM, jobject holder);

JNI_JSTRING_RETURN JNI_FUNCTION(getFFmpegInfo)(JNI_DEFAULT_PARAM);

struct JNI_GLOBAL_INFO {
    JavaVM *vm;
};
JNI_GLOBAL_INFO jni_global_info;
#endif //XPLAY_NATIVE_LIB_H_H
