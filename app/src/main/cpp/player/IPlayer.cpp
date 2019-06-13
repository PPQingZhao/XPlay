

#include "IPlayer.h"
#include "../demux/IDemux.h"
#include "../decode/IDecode.h"
#include "../audio/IResample.h"
#include "../videoviewer/IVideoView.h"
#include "../audio/IAudioPlay.h"
#include "../log/XLog.h"

IPlayer *IPlayer::Get() {
    return new IPlayer();
}

void IPlayer::Main() {
    while (!isExit) {
        if (IsPausing()) {
            XSleep(2);
            continue;
        }
        mutex.lock();
        if (!audioPlay) {
            if (vDecode) {
                vDecode->synPts = vDecode->pts;
            }
            mutex.unlock();
            XSleep(2);
            continue;
        }

        //同步: 获取音频的pts告诉视频
        int apts = audioPlay->pts;
        vDecode->synPts = apts;
//        aDecode->synPts = apts;
//        XLOGE("apts %d", apts);
        mutex.unlock();
        XSleep(2);
    }
}

void IPlayer::Close() {
    mutex.lock();
    //①先关闭主体线程,再清理观察者
    //音视频同步线程
    XLOGE("@@@@@@@@@@@@@@@@@ IPlayer->Stop()");
    XThread::Stop();
    //解封装清理
    if (demux) {
        XLOGE("@@@@@@@@@@@@@@@@@ demux->Stop()");
        demux->Stop();
    }
    //解码线程
    if (vDecode) {
        XLOGE("@@@@@@@@@@@@@@@@@ vDecode->Stop()");
        vDecode->Stop();
    }
    if (aDecode) {
        XLOGE("@@@@@@@@@@@@@@@@@ aDecode->Stop()");
        aDecode->Stop();
    }
    if (audioPlay) {
        XLOGE("@@@@@@@@@@@@@@@@@ audioPlay->Stop()");
        audioPlay->Stop();
    }
    //②清理缓冲队列
    if (vDecode) {
        vDecode->Clear();
    }
    if (aDecode) {
        aDecode->Clear();
    }
    if (audioPlay) {
        audioPlay->Clear();
    }
    //③清理资源
    if (audioPlay) {
        audioPlay->Close();
    }
    if (videoView) {
        videoView->Close();
    }
    if (vDecode) {
        vDecode->Close();
    }
    if (aDecode) {
        aDecode->Close();
    }
    if (demux) {
        demux->Close();
    }
    mutex.unlock();
}

double IPlayer::PlayPos() {
    double pos = 0.0;
    mutex.lock();
    if (demux) {
        if (demux->totalMs > 0) {
            if (vDecode) {
                pos = (double) vDecode->pts / (double) demux->totalMs;
            }
        }
    }
    mutex.unlock();
    return pos;
}

bool IPlayer::Open(const char *path) {
    Close();
    mutex.lock();
    //解封装
    if (!demux || !demux->Open(path)) {
        mutex.unlock();
        XLOGE("demux->Open failed %s", path);
        return false;
    }
    //视频解码．解码可能不需要(如果解封后就是原始数据)
    if (!vDecode || !vDecode->Open(demux->GetVideoParameter(), isHardDecode ? DECODE_HARD
                                                                            : DECODE_SOFT)) {
        XLOGE("vDecode->Open fialed %s ", path);
//        return false;
    }
    if (!aDecode || !aDecode->Open(demux->GetAudioParameter(), DECODE_SOFT)) {
        XLOGE("aDecode->Open fialed %s ", path);
//        return false;
    }
    //重采样　有可能不需要，解码或者解封后就是直接能播放的数据
    aOutPara = demux->GetAudioParameter();
    if (!resample || !resample->Open(demux->GetAudioParameter(), aOutPara)) {
        XLOGE("resample->Open fialed %s ", path);
    }
    mutex.unlock();
    return true;
}

bool IPlayer::Seek(double pos) {
    bool ret = false;
    //在seek之前暂停线程
    SetPause(true);
    mutex.lock();
    if (!demux) return ret;
    //②清理缓冲队列
    if (vDecode) {
        //清理了本地缓冲队列和ffmpeg缓冲队列
        vDecode->Clear();
    }
    if (aDecode) {
        aDecode->Clear();
    }
    if (audioPlay) {
        audioPlay->Clear();
    }
    //跳转到关键帧
    ret = demux->Seek(pos);
    //解码到实际显示的帧
    int seekPos = pos * demux->totalMs;
    if (vDecode) { //存在视频才读取
        while (!isExit) {
            XData pkt = demux->Read();
            if (pkt.size <= 0)break;
            if (pkt.decodeType == DECODE_TYPE_AUDIO) {
                //当前位置小于seek位置
                if (pkt.pts < seekPos) {
                    pkt.Drop();
                    continue;
                }
                //通知观察者,写入缓冲队队列
                demux->observable->Notify(pkt);
                continue;
            }
            //解码需要显示的帧之前的数据
            vDecode->SendPacket(pkt);
            pkt.Drop();
            XData data = vDecode->ReceivedFrame();
            if (data.size < 0) {
                continue;
            }
            if (data.pts >= seekPos) {
                //不能在这个线程通知解码器,因为创建解码器中的egl线程与当前线程不一致
//                vDecode->observable->Notify(data);
                break;
            }
        }
    }
    mutex.unlock();
    //在seek之后恢复线程
    SetPause(false);
    return ret;


}

void IPlayer::SetPause(bool isP) {
    mutex.lock();
    XLOGE("@@@@@@@@@@@@  IPlayer::SetPause()");
    XThread::SetPause(isP);
    if (demux) {
        XLOGE("@@@@@@@@@@@@  demux::SetPause()");
        demux->SetPause(isP);
    }
    if (vDecode) {
        XLOGE("@@@@@@@@@@@@  vDecode::SetPause()");
        vDecode->SetPause(isP);
    }
    if (aDecode) {
        XLOGE("@@@@@@@@@@@@  aDecode::SetPause()");
        aDecode->SetPause(isP);
    }
    if (audioPlay) {
        XLOGE("@@@@@@@@@@@@  audioPlay::SetPause()");
        audioPlay->SetPause(isP);
    }
    mutex.unlock();
}

bool IPlayer::Start() {
    mutex.lock();
    if (audioPlay) {
        XLOGE("@@@@@@@@@@@@  audioPlay::Start()");
        audioPlay->StartPlay(aOutPara);
    }
    //先启动音频解码，再次启动视频解码
    if (aDecode && aDecode->isOpen) {
        XLOGE("@@@@@@@@@@@@  aDecode::Start()");
        aDecode->Start();
    }

    if (vDecode && vDecode->isOpen) {
        XLOGE("@@@@@@@@@@@@  vDecode::Start()");
        vDecode->Start();
    }

    XLOGE("@@@@@@@@@@@@  demux::Start()");
    //启动解封装线程
    if (!demux || !demux->isOPen || !demux->Start()) {
        XLOGE("demux->Start() failed");
        mutex.unlock();
        Close();
        return false;
    }
    XLOGE("@@@@@@@@@@@@  IPlayer::Start()");
    //开启音频视频同步线程；
    XThread::Start();
    mutex.unlock();
    return true;
}

bool IPlayer::InitView(void *win) {
    if (videoView) {
//        videoView->Close();
        videoView->SetRender(win);
    }
    return true;
}