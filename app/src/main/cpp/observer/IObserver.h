

#ifndef XPLAY_IOBSERVER_H
#define XPLAY_IOBSERVER_H

#include "../data/XData.h"

class IObserver{
public:
    //纯虚构函数
    virtual void Update(XData data) = 0;
};
#endif //XPLAY_IOBSERVER_H
