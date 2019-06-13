//
// Created by qing on 19-1-5.
//

#ifndef XPLAY_XSHADER_H
#define XPLAY_XSHADER_H

#include <mutex>

enum XShaderType {
    XSHADER_yuv420P = 0, // Y:u:v = 4:1:1
    XSHADER_NV12 = 25,   // Y:uv = 4:1
    XSHADER_NV21 = 26,   // Y:vu = 4:1
};

class XShader {
public:
    virtual bool Init(XShaderType type = XSHADER_yuv420P);
    virtual void Close();
    /**
     *  获取材质并映射到内存
     * @param index　对应下标
     * @param width　
     * @param height
     * @param buf
     * @param isa  是否有透明通道 （默认false）
     */
    void GetTexture(unsigned int index,
                    int width,
                    int height,
                    unsigned char *buf,
                    bool isa = false);

    virtual void Draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
    std::mutex mux;
};


#endif //XPLAY_XSHADER_H
