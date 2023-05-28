//
// Created by dev on 2023/5/28.
//

#include "TextureMapSample.h"
#include "GLUtils.h"
#include "LogUtils.h"

#define vertex_pos_index 0
#define texture_pos_index 1
#define TEXTURE_COUNT 1  // 纹理数量

TextureMapSample::TextureMapSample() {

    m_TextureId = 0;


}

TextureMapSample::~TextureMapSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void TextureMapSample::Init() {
    // step 1: 生成一个纹理id ,将纹理id赋值给 m_TextureId；
    glGenTextures(TEXTURE_COUNT, &m_TextureId);
    // step 2: 将m_TextureId bind 到 GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    // step 3: 设置 s，t轴 拉伸截取方式
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // s 轴
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // t 轴
    // step 4: 设置纹理采样的方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // step 5: 解绑
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    // layout(location = 1) in vec2 a_texCoord ： 纹理坐标
    // out vec2 v_texCoord : 是输出坐标的纹理坐标
    char vShaderStr[] =
            "#version 300 es                            \n"
            "layout(location = 0) in vec4 a_position;   \n"
            "layout(location = 1) in vec2 a_texCoord;   \n"
            "out vec2 v_texCoord;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = a_position;               \n"
            "   v_texCoord = a_texCoord;                \n"
            "}                                          \n";


    //  out vec2 v_texCoord ==数据给===> in vec2 v_texCoord
    // out vec4 outColor : 输出的颜色
    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec2 v_texCoord;                                 \n"
            "layout(location = 0) out vec4 outColor;             \n"
            "uniform sampler2D s_TextureMap;                     \n"
            "void main()                                         \n"
            "{                                                   \n"
            "  outColor = texture(s_TextureMap, v_texCoord);     \n"
            "  //outColor = texelFetch(s_TextureMap,  ivec2(int(v_texCoord.x * 404.0), int(v_texCoord.y * 336.0)), 0);\n"
            "}                                                   \n";


    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    if (!m_ProgramObj) {
        m_SampleLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
    } else {
        LOGCATE("TextureMapSample::Init create program fail");
    }

}

void TextureMapSample::LoadImage(NativeImage *pImage) {
    if (pImage == nullptr) {
        return;
    }
    LOGCATI("TextureMapSample::LoadImage  设置图片数据 : pImage = %p", pImage->ppPlane[0]);
    m_RenderImage.height = pImage->height;
    m_RenderImage.width = pImage->width;
    m_RenderImage.format = pImage->format;
    LOGCATI("TextureMapSample::   设置图片数据 step");
    // todo ?
    // m_RenderImage.ppPlane = pImage->ppPlane;
    // 拷贝 image 数据
    NativeImageUtil::copyNativeImage(pImage, &m_RenderImage);
    LOGCATI("TextureMapSample::LoadImage  设置图片数据 :end");
}


void TextureMapSample::Draw(int screenW, int screenH) {
    if (m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) {
        return;
    }
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float defaultColor = 1.0f;
    glClearColor(defaultColor, defaultColor, defaultColor, defaultColor);

    // 顶点坐标
    GLfloat vertexCoordinate[] = {
            -1.0f, 0.5f, 0.0f,  // Position 0
            -1.0f, -0.5f, 0.0f,  // Position 1
            1.0f, -0.5f, 0.0f,   // Position 2
            1.0f, 0.5f, 0.0f,   // Position 3
    };
    // 纹理坐标
    GLfloat textureCoordinate[] = {
            0.0f, 0.0f,        // TexCoord 0
            0.0f, 1.0f,        // TexCoord 1
            1.0f, 1.0f,        // TexCoord 2
            1.0f, 0.0f         // TexCoord 3

    };
    // 一个矩形，有4个顶点，绘制顺序可以为2步，
    // step 1 : v0,v1,v2:画一个三角形 t1;
    // step 2 : v0,v2,v3:画一个三角形 t2;
    // step 3 : t1和t2 组成一个矩形；

    // 顶点index
    GLushort indices[] = {0, 1, 2,
                          0, 2, 3};
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    // 解绑
    glBindTexture(GL_TEXTURE_2D, GL_NONE);


    glUseProgram(m_ProgramObj);
    // 加载顶点坐标，0 是在shader 定义好的： layout(location = 0) in vec4 a_position;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertexCoordinate);

    // 加载纹理坐标 1：  "layout(location = 1) in vec2 a_texCoord;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), textureCoordinate);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    // bind the rgba map;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_TextureId);

    //设置 m_SampleLoc 位置的值
    glUniform1i(m_SampleLoc,0);
    // 画了啊
    // mode   :  指定绘制图元的类型
    // count  :  为绘制图元的数量乘上一个图元的顶点数。
    //        : 表示要从索引缓冲区中读取多少个元素来进行绘制
    //        :（指定绘制的顶点个数）。
    // type为 : 索引值的类型,其实就是 indices 数据
    // indices : 指向索引存贮位置的指针。
    // NOTE : 使用6，是因为两个三角形，有6个顶点；
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,indices);
}


void TextureMapSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        // 删除纹理，释放内存；
        glDeleteTextures(TEXTURE_COUNT, &m_TextureId);
    }

}
