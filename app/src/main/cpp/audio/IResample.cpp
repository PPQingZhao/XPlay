//
// Created by qing on 19-1-21.
//

#include "IResample.h"
#include "../observer/IObserver.h"
#include "../log/XLog.h"

class ResampleObserver : public IObserver {
public:
    void Update(XData data) {
//        XLOGE("ResampleObserver update().");
        XData resampleData = resample->Resample(data);
        if (resampleData.size > 0) {
            resample->observable->Notify(resampleData);
        }
    }

    IResample *resample;

    ResampleObserver(IResample *resample) {
        this->resample = resample;
    }

};

IResample::IResample() {
    this->resampleObserver = new ResampleObserver(this);
}