//
// Created by qing on 19-1-5.
//

#ifndef XPLAY_XTEXTURE_H
#define XPLAY_XTEXTURE_H

enum XTextureType {
    XTEXTURE_YUV420P = 0, // Y:u:v = 4:1:1
    XTEXTURE_YUVJ420P = 12, // Y:u:v = 4:1:1
    XTEXTURE_NV12 = 25,   // Y:uv = 4:1
    XTEXTURE_NV25 = 26,   // Y:vu = 4:1
};

class XTexture {
public:
    static XTexture *GetInstance();

    virtual bool Init(void *win, XTextureType type = XTEXTURE_YUV420P) = 0;

    virtual void Draw(unsigned char *data[], int width, int height)=0;
    virtual void Drop() = 0;
    virtual ~XTexture(){};
protected:
    XTexture(){}
};


#endif //XPLAY_XTEXTURE_H
