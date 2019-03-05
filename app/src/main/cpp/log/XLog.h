//
// Created by qing on 18-10-23.
//

#ifndef XPLAY_XLOG_H
#define XPLAY_XLOG_H



class XLog {

};


#ifdef ANDROID
#include <android/log.h>
#define XLOGE(...) __android_log_print(ANDROID_LOG_ERROR,"XPlay",__VA_ARGS__)
#define XLOGI(...) __android_log_print(ANDROID_LOG_INFO,"XPlay",__VA_ARGS__)
#else
#define XLOGI(...) printf("XPlay",__VA_ARGS__)
#endif
#endif //XPLAY_XLOG_H
