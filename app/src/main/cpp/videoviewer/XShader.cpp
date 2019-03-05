//
// Created by qing on 19-1-5.
//

#include "XShader.h"
#include "../log/XLog.h"
#include <GLES2/gl2.h>
//顶点着色器glsl
#define GET_STR(x) #x
static const char *vertexShader = GET_STR(
        attribute
        vec4 aPosition;  //顶点坐标
        attribute
        vec2 aTexCoord;　//材质顶点坐标
        varying vec2 vTexCoord;　　//输出的材质坐标
        void main(){
        vTexCoord = vec2(aTexCoord.x, 1.0-aTexCoord.y);
        gl_Position = aPosition;
}
);

//片元着色器  软解码和部分x86硬解码　使用下面这种格式
static const char *fragYUV_420P = GET_STR(
        precision　mediump float; //精度
        varying　vec2 vTexCoord;　//顶点着色器传递的坐标
        uniform sampler2D yTexture;　　//输入的材质(不透明灰度，单像素)
        uniform sampler2D uTexture;　
        uniform sampler2D vTexture;
        void main(){
        vec3 yuv;
        vec3 rgb;
        yuv.r = texture2D(yTexture, vTexCoord).r;
        yuv.g = texture2D(uTexture, vTexCoord).r - 0.5;
        yuv.b = texture2D(vTexture, vTexCoord).r - 0.5;
        //yuv 转换　rgb   一个矩阵 mat3
        rgb = mat3(1.0, 1.0, 1.0,
        0.0, -0.39465, 2.03211,
        1.13983, -0.5806, 0.0)*yuv;
        //输出像素颜色
        gl_FragColor = vec4(rgb, 1.0);
}
);

//片元着色器  软解码和部分x86硬解码　使用下面这种格式
static const char *fragNV12 = GET_STR(
        precision　mediump float; //精度
        varying　vec2 vTexCoord;　//顶点着色器传递的坐标
        uniform sampler2D yTexture;　　//输入的材质(不透明灰度，单像素)
        uniform sampler2D uvTexture;　
        void main(){
        vec3 yuv;
        vec3 rgb;
        yuv.r = texture2D(yTexture, vTexCoord).r;
        yuv.g = texture2D(uvTexture, vTexCoord).r - 0.5;
        yuv.b = texture2D(uvTexture, vTexCoord).a - 0.5;
        //yuv 转换　rgb   一个矩阵 mat3
        rgb = mat3(1.0, 1.0, 1.0,
        0.0, -0.39465, 2.03211,
        1.13983, -0.5806, 0.0)*yuv;
        //输出像素颜色
        gl_FragColor = vec4(rgb, 1.0);
}
);


//shader　初始化
static GLuint InitShader(const char *code, GLint type) {
    GLuint sh = glCreateShader(type);
    if (sh == 0) {
        XLOGE("====>> InitShader failed! %d", type);
        return 0;
    }
    //加载shader
    glShaderSource(sh,
                   1, //shader　数量
                   &code,//shader代码
            //代码长度
                   0);
    //编译shader
    glCompileShader(sh);

    //获取编译情况
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        XLOGE("glGetShaderiv　failed!");
        return 0;
    }
    XLOGE("glGetShaderiv　succee!");
    return sh;
}

bool XShader::Init(XShaderType type) {
    XLOGE("Enter XShader::Init()!");
    //shader　初始化
    //顶点坐标初始化
    vsh = InitShader(vertexShader, GL_VERTEX_SHADER);
    if (vsh == 0) {
        XLOGE("InitShader GL_VERTEX_SHADER failed!");
        return false;
    }
    //片元初始化
    switch (type) {
        case XSHADER_yuv420P: //片元yuv420　shader初始化
        case XSHADER_YUVJ420P:
            fsh = InitShader(fragYUV_420P, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV12:  // NV12
            fsh = InitShader(fragNV12, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV21:
            fsh = InitShader(fragNV12, GL_FRAGMENT_SHADER);
            break;
        default:
            XLOGE("XShaderType format is error!");
            break;
    }

    if (fsh == 0) {
        XLOGE("InitShader GL_FRAGMENT_SHADER　failed!");
        return false;
    }

    //创建渲染程序
    program = glCreateProgram();
    if (program == 0) {
        XLOGE("===========>>glCreateProgram failed! ");
        return false;
    }
    //渲染程序中加入着色器代码
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    //连接程序
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        XLOGE("=========>> glLinkProgram failed!");
        return false;
    }

    //激活渲染程序
    glUseProgram(program);
    /////////////////////////////////////////////////////
    //加入三维顶点数据　两个三角形组成正方形
    static float ver[] = {
            1.0f, -1.f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f
    };

    GLuint apos = glGetAttribLocation(program, "aPosition");
    glEnableVertexAttribArray(apos);
    //传递值
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12, ver);

    //加入材质坐标
    static float txts[] = {
            1.0f, 0.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
    };

    GLuint atex = glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(atex);
    glVertexAttribPointer(atex, 2, GL_FLOAT, GL_FALSE, 8, txts);

    //材质纹理初始化
    //设置纹理层
    glUniform1i(glGetUniformLocation(program, "yTexture"), 0); //纹理层第一层
    switch (type) {
        case XSHADER_yuv420P:
        case XSHADER_YUVJ420P:
            glUniform1i(glGetUniformLocation(program, "uTexture"), 1); //纹理层第二层
            glUniform1i(glGetUniformLocation(program, "vTexture"), 2); //纹理层第三层
            break;
        case XSHADER_NV12:
        case XSHADER_NV21:
            glUniform1i(glGetUniformLocation(program, "uvTexture"), 1); //纹理层第二层
            break;
    }

    XLOGE("InitShader success!");
    return true;
}

void XShader::GetTexture(unsigned int index, int width, int height, unsigned char *buf, bool isa) {
//    XLOGE("XShader  width %d  height %d", width, height);
    //默认灰度图GL_LUMINANCE,透明通道GL_LUMINANCE_ALPHA
    unsigned int format = isa ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
    if (texts[index] == 0) {
        glGenTextures(1, &texts[index]);//创建纹理
        //设置纹理属性
        glBindTexture(GL_TEXTURE_2D, texts[index]);
        //缩小过滤器
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //放大过滤器
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //设置纹理格式和大小
        glTexImage2D(GL_TEXTURE_2D,
                     0,//细节基本　0默认
                     format,//gpu内部格式，灰度图
                     width, height,//尺寸是2的次方
                     0,//边框
                     format,//数据的像素格式　亮度，灰度图　要与上面一致
                     GL_UNSIGNED_BYTE,//像素的数据类型
                     NULL//纹理的数据
        );
    }
    glActiveTexture(GL_TEXTURE0 + index);//激活第index层纹理
    glBindTexture(GL_TEXTURE_2D, texts[index]);//绑定到创建的opengl纹理
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE,
                    buf);//替换纹理内容
}

void XShader::Draw() {
//    XLOGE("XShader Draw!");
    if (program == 0) {
        XLOGE("Program is NUll when call Draw of Xshader!");
        return;
    }
    //三维绘制
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}