//
// Created by qing on 18-10-23.
//

#include "IDemux.h"
#include "../log/XLog.h"

void IDemux::Main() {
    while (!isExit) {
        XData d = Read();
        if (d.size > 0) {
            observable->Notify(d);
        }
    }
};