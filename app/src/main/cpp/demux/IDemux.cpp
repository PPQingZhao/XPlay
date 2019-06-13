
#include "IDemux.h"
#include "../log/XLog.h"

void IDemux::Main() {
    while (!isExit) {
        if (IsPausing()) {
            XSleep(2);
            continue;
        }
        XData d = Read();
//        XLOGE("IDemux::Main()");
        if (d.size > 0) {
            observable->Notify(d);
            continue;
        } else {
            XSleep(2);
        }
    }
};