
#include "IObservable.h"

//添加观察者
void IObservable::AddObserver(IObserver *observer) {
    //线程安全
    if (!observer)
        return;

    this->mutex.lock();
    this->obssVector.push_back(observer);
    this->mutex.unlock();
}

//通知　所有观察者
void IObservable::Notify(XData data) {
    mutex.lock();
    for (int i = 0; i < this->obssVector.size(); ++i) {
        this->obssVector[i]->Update(data);
    }
    mutex.unlock();
}