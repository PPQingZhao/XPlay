//
// Created by qing on 19-1-21.
//

#ifndef XPLAY_IRESAMPLE_H
#define XPLAY_IRESAMPLE_H

#include "../data/XParameter.h"
#include "../data/XData.h"
#include "../observer/IObservable.h"

class IResample :public XThread{
public:
    virtual bool Open(XParameter in, XParameter out = XParameter()) = 0;
    virtual void Close() = 0;
    virtual XData Resample(XData data) =0;

    IObservable *observable = new IObservable();
    IObserver *resampleObserver;
    int outChannels = 2; //输出通道数
    int outFormat = 1;  //输出格式
    //是否打开样本采样率
    bool isOpen;
protected:
    IResample();

};


#endif //XPLAY_IRESAMPLE_H
