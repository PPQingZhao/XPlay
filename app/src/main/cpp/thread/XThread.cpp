//
// Created by qing on 18-10-24.
//

#include "XThread.h"
#include "../log/XLog.h"
#include <thread>

using namespace std; //这个尽量不要放在头文件.h中引用，会引发冲突，不可控
void XSleep(int mis) {
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}

//启动线程
bool XThread::Start() {
    isExit = false;
    //启动线程
    thread th(&XThread::ThreadMain, this);
    //当前线程放弃对新建的线程的控制
    th.detach();
    return true;
}

void XThread::ThreadMain() {
    isRunning = true;
    XLOGE("===========>> 线程函数进入");
    Main();
    XLOGE("===========>> 线程函数退出");
    isRunning = false;
}

//通过控制isExit　安全停止线程（不一定成功）
void XThread::Stop() {
    isExit = true;
    for (int i = 0; i < 200; i++) {
        if (!isRunning) {
            XLOGE("Stop 停止线程成功！");
            return;
        } else {
            XSleep(1);
            continue;
        }
    }
    XLOGE("Stop 超时");
}