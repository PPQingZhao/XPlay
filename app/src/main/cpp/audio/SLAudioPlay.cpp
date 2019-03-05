//
// Created by qing on 19-1-21.
//

#include "SLAudioPlay.h"
#include "../log/XLog.h"
#include <SLES/OpenSLES.h>          //音频播放器
#include <SLES/OpenSLES_Android.h>　//音频播放器
#include <stdio.h>

static SLObjectItf slObjectItf = NULL;
static SLEngineItf slEngineItf;
static SLObjectItf mix = NULL;
static SLObjectItf player = NULL;
static SLPlayItf slPlayItf = NULL;
static SLAndroidSimpleBufferQueueItf simpleBufferQueueItf_player = NULL;

SLAudioPlay::~SLAudioPlay() {
    delete this->buf;
}

SLAudioPlay::SLAudioPlay() {
    this->buf = new unsigned char[1024 * 1024];
}

//音频播放器引擎初始化
static SLEngineItf CreateSL() {
    SLresult sLresult;
    SLEngineItf slEngineItf = NULL;

    //创建引擎对象
    sLresult = slCreateEngine(&slObjectItf, 0, 0, 0, 0, 0);
    if (sLresult != SL_RESULT_SUCCESS)//创建失败
        return NULL;

    //实例化
    sLresult = (*slObjectItf)->Realize(slObjectItf, SL_BOOLEAN_TRUE);
    if (sLresult != SL_RESULT_SUCCESS) {
        return NULL;
    }

    //获取接口
    sLresult = (*slObjectItf)->GetInterface(slObjectItf, SL_IID_ENGINE, &slEngineItf);
    if (sLresult != SL_RESULT_SUCCESS) {
        return NULL;
    }
    return slEngineItf;
}

void SLAudioPlay::PlayCall(void *bufq) {
    if (!bufq) return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf) bufq;
//    XLOGE("PlayCall success!");
    XData d = GetData();
    if (d.size <= 0) {
        XLOGE("AudioPlay GetData() size is 0!");
        return;
    }
    if (!buf)
        return;
    memcpy(buf,d.data,d.size);
    (*bf)->Enqueue(bf, buf, d.size);
    d.Drop();
}

static void
PcmCallBack(SLAndroidSimpleBufferQueueItf slAndroidSimpleBufferQueueItf, void *context) {
    SLAudioPlay *audioPlay = (SLAudioPlay *) context;
    if (!audioPlay) {
        XLOGE("PcmCallBack failed contex is null!");
        return;
    }
    audioPlay->PlayCall((void *) slAndroidSimpleBufferQueueItf);
}

bool SLAudioPlay::StartPlay(XParameter out) {
    //①创建引擎
    slEngineItf = CreateSL();
    if (!slEngineItf) {
        XLOGE("===========>> CreatedSL failed! ");
        return false;
    }

    //②创建混音器  mix
    SLresult slResult = 0;
    slResult = (*slEngineItf)->CreateOutputMix(slEngineItf, &mix, 0, 0, 0);
    if (slResult != SL_RESULT_SUCCESS) {
        XLOGE("==============>> CreateOutputMix failed!");
        return false;
    }

    //实例化 第二个参数：表示阻塞式等待创建
    slResult = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (slResult != SL_RESULT_SUCCESS) {
        XLOGE("==============>> Realize failed!");
        return false;
    }

    //参数１: 输出类型  参数2: 混音器
    SLDataLocator_OutputMix slDataLocator_outputMix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    //
    SLDataSink slDataSink = {&slDataLocator_outputMix, NULL};

    //③ 配置音频信息
    //缓冲队列 10个长度
    SLDataLocator_AndroidSimpleBufferQueue simpleBufferQueue = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    //具体的音频格式
    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM,                              //pcm格式
                            (SLuint32) out.channels,                                   //声道数
                            (SLuint32) out.sample_rate * 1000,                         //采样率
                            SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_PCMSAMPLEFORMAT_FIXED_16,                     //单个容器的大小
                            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,  //前左声道　　前右声道
                            SL_BYTEORDER_LITTLEENDIAN};                     //字节序,小端

    SLDataSource slDataSource = {&simpleBufferQueue, &pcm};

    //④ 创建播放器
    //创建对象
    const SLInterfaceID slInterfaceID[] = {SL_IID_BUFFERQUEUE};
    const SLboolean sLboolean[] = {SL_BOOLEAN_TRUE};

    slResult = (*slEngineItf)->CreateAudioPlayer(slEngineItf,
                                                 &player,
                                                 &slDataSource,
                                                 &slDataSink,
                                                 sizeof(slInterfaceID) / sizeof(sLboolean),
                                                 slInterfaceID,
                                                 sLboolean);

    if (slResult != SL_RESULT_SUCCESS) {
        XLOGE("==================>> CreateAudioPlayer failed!");
        return false;
    }
    //　实例化播放器对象
    slResult = (*player)->Realize(player, SL_BOOLEAN_FALSE);
    if (slResult != SL_RESULT_SUCCESS) {
        XLOGE("==========>> (*player)->Realize failed!");
        return false;
    }
    //获取player　接口
    slResult = (*player)->GetInterface(player, SL_IID_PLAY, &slPlayItf);
    if (slResult != SL_RESULT_SUCCESS) {
        XLOGE("================>> (*player)->GetInterface  SL_IID_PLAY failed!");
        return false;
    }
    //获取　队列
    slResult = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &simpleBufferQueueItf_player);
    if (slResult != SL_RESULT_SUCCESS) {
        XLOGE("================>> (*player)->GetInterface SL_IID_BUFFERQUEUE failed!");
        return false;
    }

    //设置回调函数，播放队列空调用
    slResult = (*simpleBufferQueueItf_player)->RegisterCallback(simpleBufferQueueItf_player,
                                                                PcmCallBack, this);
    if (slResult != SL_RESULT_SUCCESS) {
        XLOGE("================>>  (*simpleBufferQueueItf_player)->RegisterCallback failed!");
        return false;
    }

    //设置播放器为播放状态
    (*slPlayItf)->SetPlayState(slPlayItf, SL_PLAYSTATE_PLAYING);
    //启动队列回调  插入一个字节
    (*simpleBufferQueueItf_player)->Enqueue(simpleBufferQueueItf_player, "", 1);
    XLOGE("StartPlay success!");
    return true;
}