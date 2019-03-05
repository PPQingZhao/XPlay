//
// Created by qing on 18-12-26.
//

#ifndef XPLAY_IOBSERVABLE_H
#define XPLAY_IOBSERVABLE_H

#include "../data/XData.h"
#include "../thread/XThread.h"
#include "IObserver.h"
#include <vector> //c++　11才支持
#include <mutex>    //c++ 11　才支持

//单例模式
class IObservable{
public:
    //添加观察者 (线程安全)
    void AddObserver(IObserver *observer);

    //通知　所有观察者　(线程安全)
    void Notify(XData data);

protected:
    //用于保存观察者对象
    std::vector<IObserver *> obssVector;
    std::mutex mutex;
};

#endif //XPLAY_IOBSERVER_H
