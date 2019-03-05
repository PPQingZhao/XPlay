//
// Created by qing on 19-1-5.
//

#include "GLVideoView.h"
#include "../log/XLog.h"
#include "XTexture.h"

void GLVideoView::SetRender(void *win) {
    this->view = win;
}

void GLVideoView::Render(XData data) {
    if (!view) {
        XLOGE("Render view is NULL!");
        return;
    }
    if (!texture) {
        texture = XTexture::GetInstance();
        texture->Init(view, (XTextureType) data.format);
    }
    texture->Draw(data.datas, data.width, data.height);
}