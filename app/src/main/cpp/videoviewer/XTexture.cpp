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

    bool Init(void *win, XTextureType type) {
        this->textureType = type;
        XLOGE("Enter CXTexture Init()!");
        if (!win) {
            XLOGE("XTexture init failed!");
            return false;
        }
        if (!xegl)
            xegl = XEGL::GetIntance();
        if (!xegl->Init(win)) {
            XLOGE("XEGL::GetIntance()->Init(win) failed!");
            return false;
        }
        bool ret = shader.Init((XShaderType) type);
        XLOGE("XTexture init success!");
        return true;
    }

    void Draw(unsigned char *data[], int width, int height) {
//        XLOGE("XTexture Draw!");
//        XLOGE("width %d  height %d", width, height);
        shader.GetTexture(0, width, height, data[0]);      //y
        if (XSHADER_yuv420P == textureType
            || XSHADER_YUVJ420P == textureType) {
            shader.GetTexture(1, width / 2, height / 2, data[1]);  //u
            shader.GetTexture(2, width / 2, height / 2, data[2]);  //v
        } else {
            shader.GetTexture(1, width / 2, height / 2, data[1], true); //uv
        }
        shader.Draw();
        if (xegl) {
            xegl->Draw();
        }
    }
};

XTexture *XTexture::GetInstance() {
    return new CXTexture();
}