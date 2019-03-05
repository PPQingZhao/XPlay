//
// Created by qing on 19-1-3.
//

#ifndef XPLAY_XEGL_H
#define XPLAY_XEGL_H

//单例模式
class XEGL {
public:
    virtual bool Init(void *win) = 0;

    static XEGL *GetIntance();

    virtual void Draw() = 0;

protected:
    XEGL() {}
};


#endif //XPLAY_XEGL_H
