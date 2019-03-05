//
// Created by qing on 18-12-26.
//

#ifndef XPLAY_TESTOBSERVER_H
#define XPLAY_TESTOBSERVER_H

#include "../data/XData.h"
#include "IObserver.h"

//#include "IObserver.h"
class TestObserver:public IObserver{
public:
     void Update(XData data);
};

#endif //XPLAY_TESTOBSERVER_H
