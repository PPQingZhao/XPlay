//
// Created by qing on 18-10-24.
//

#ifndef XPLAY_XTHREAD_H
#define XPLAY_XTHREAD_H

//sleep  毫秒
void XSleep(int mis);
//c++ 11线程库
class XThread {
public:
    //启动线程
    virtual bool Start();

    //通过控制isExit　安全停止线程（不一定成功）
    virtual void Stop();

    //入口主函数
    virtual void Main(){}

    bool isRunning = false;
    bool isExit = false; //是否退出线程

private:
    void ThreadMain();
};


#endif //XPLAY_XTHREAD_H
