//
// Created by qing on 19-1-5.
//

#include "XTexture.h"
#include "../log/XLog.h"
#include "XEGL.h"
#include "XShader.h"

class CXTexture : public XTexture {
public:
    XShader shader;
    XTextureType textureType;
    XEGL *xegl = 0;
    std::mutex mux;

    virtual void Drop() {
        mux.lock();
        xegl->Close();
        shader.Close();
        mux.unlock();
        //删除当前对象
        delete (this);
    }

    virtual bool Init(void *win, XTextureType type) {
        mux.lock();
        if (xegl) {
            xegl->Close();
        }
        shader.Close();
        this->textureType = type;
        XLOGE("Enter CXTexture Init()!");
        if (!win) {
            mux.unlock();
            XLOGE("XTexture init failed!");
            return false;
        }
        if (!xegl)
            xegl = XEGL::GetIntance();
        if (!xegl->Init(win)) {
            XLOGE("XEGL::GetIntance()->Init(win) failed!");
            mux.unlock();
            return false;
        }
        bool ret = shader.Init((XShaderType) type);
        mux.unlock();
        XLOGE("XTexture init success!");
        return true;
    }

    virtual void Draw(unsigned char *data[], int width, int height) {
//        XLOGE("XTexture Draw!");
//        XLOGE("width %d  height %d", width, height);
        mux.lock();
        shader.GetTexture(0, width, height, data[0]);      //y
        if (XSHADER_yuv420P == textureType) {
            shader.GetTexture(1, width / 2, height / 2, data[1]);  //u
            shader.GetTexture(2, width / 2, height / 2, data[2]);  //v
        } else {
            shader.GetTexture(1, width / 2, height / 2, data[1], true); //uv
        }
        shader.Draw();
        if (xegl) {
            xegl->Draw();
        }
        mux.unlock();
    }
};

XTexture *XTexture::GetInstance() {
    return new CXTexture();
}