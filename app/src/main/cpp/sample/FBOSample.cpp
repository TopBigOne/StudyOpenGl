//
// Created by dev on 2023/6/1.
//

#include "FBOSample.h"
#include "GLUtils.h"

FBOSample::FBOSample() {
    LOGCATD("FBOSample ");
    m_VaoIds[0] = GL_NONE;
    m_VboIds[0] = GL_NONE;

    m_ImageTextureId = GL_NONE;
    m_FboTextureId   = GL_NONE;
    m_SampleLoc      = GL_NONE;

    // for fbo
    m_FboId             = GL_NONE;
    m_FboProgramObj     = GL_NONE;
    m_FboFragmentShader = GL_NONE;
    m_FboSamplerLoc     = GL_NONE;

}

void FBOSample::LoadImage(NativeImage *p_Image) {
    LOGCATD("FBOSample ::LoadImage ");
    if (p_Image == nullptr) {
        return;
    }
    m_RenderImage.height = p_Image->height;
    m_RenderImage.width  = p_Image->width;
    m_RenderImage.format = p_Image->format;
    NativeImageUtil::copyNativeImage(p_Image, &m_RenderImage);
}

void FBOSample::Init() {
    LOGCATD("FBOSample ::Init ");
    // 顶点坐标系
    GLfloat vVertices[]    = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
    };
    // 正常 纹理坐标系：原点位于 左上角
    GLfloat vTexCoors[]    = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
    };
    // fbo 纹理坐标系： 原点位于左下角
    GLfloat vFboTexCoors[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
    };

    // 指示小标，vao使用
    GLushort indices[] = {0, 1, 2, 1, 3, 2};

    char vShaderStr[] =
                 "#version 300 es                            \n"
                 "layout(location = 0) in vec4 a_position;   \n"
                 "layout(location = 1) in vec2 a_texCoord;   \n"
                 "out vec2 v_texCoord;                       \n"
                 "void main()                                \n"
                 "{                                          \n"
                 "   gl_Position = a_position;               \n"
                 "   v_texCoord = a_texCoord;                \n"
                 "}    "
                 ""
                 "                                      \n";

    // 用于普通渲染的片段着色器脚本，简单纹理映射
    char fShaderStr[] =
                 "#version 300 es\n"
                 "precision mediump float;\n"
                 "in vec2 v_texCoord;\n"
                 "layout(location = 0) out vec4 outColor;\n"
                 "uniform sampler2D s_TextureMap;\n"
                 "void main()\n"
                 "{\n"
                 "    outColor = texture(s_TextureMap, v_texCoord);\n"
                 "}";

    // 用于离屏渲染的片段着色器脚本，取每个像素的灰度值
    char fFboShaderStr[] =
                 "#version 300 es\n"
                 "precision mediump float;\n"
                 "in vec2 v_texCoord;\n"
                 "layout(location = 0) out vec4 outColor;\n"
                 "uniform sampler2D s_TextureMap;\n"
                 "void main()\n"
                 "{\n"
                 "    vec4 tempColor = texture(s_TextureMap, v_texCoord);\n"
                 "    float luminance = tempColor.r * 0.299 + tempColor.g * 0.587 + tempColor.b * 0.114;\n"
                 "    outColor = vec4(vec3(luminance), tempColor.a);\n"
                 "}"; // 输出灰度图

    // 编译链接用于普通渲染的着色器程序
    m_ProgramObj    = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader,
                                             m_FragmentShader);
    m_FboProgramObj = GLUtils::CreateProgram(vShaderStr, fFboShaderStr, m_FboVertexShader,
                                             m_FboFragmentShader);

    if (m_ProgramObj == GL_NONE || m_FboProgramObj == GL_NONE) {
        LOGCATE("FBOSample::Init m_ProgramObj == GL_NONE");
        return;
    }

    m_SampleLoc     = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
    m_FboSamplerLoc = glGetUniformLocation(m_FboProgramObj, "s_TextureMap");

    // generate vbo
    glGenBuffers(4, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vTexCoors), vTexCoors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vFboTexCoors), vFboTexCoors, GL_STATIC_DRAW);

    // 我理解的是指示器
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GO_CHECK_GL_ERROR();
    // 生成2个VAO 纹理 ，一个用于普通渲染，一个用于离屏渲染
    glGenVertexArrays(2, m_VaoIds);
    // 绑定用于普通渲染的 vao 顶点
    glBindVertexArray(m_VaoIds[0]);

    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(VERTEX_POS_INDEX); // 顶点
    glVertexAttribPointer(VERTEX_POS_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glEnableVertexAttribArray(TEXTURE_POS_INDEX);
    glVertexAttribPointer(TEXTURE_POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    // 指示器下标
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[3]);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(GL_NONE);

    // 初始化 用于离屏渲染的VAO
    glBindVertexArray(m_VaoIds[1]);// 绑定顶点

    // 处理buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(VERTEX_POS_INDEX);
    // 解释顶点数据
    glVertexAttribPointer(VERTEX_POS_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),(const void *) 0);
    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    // bind vbo 2.
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[2]);//index 2： 用于离屏渲染
    glEnableVertexAttribArray(TEXTURE_POS_INDEX);
    glVertexAttribPointer(TEXTURE_POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),(const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    // bind vbo 3,for index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[3]);
    GO_CHECK_GL_ERROR();
    // unbind顶点
    glBindVertexArray(GL_NONE);

    // 创建并初始化纹理（不是 fbo 纹理哦）
    glGenTextures(1, &m_ImageTextureId);
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId); // 记得解绑
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA,GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    // 解绑
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    GO_CHECK_GL_ERROR();

    if (!CreateFrameBufferObj()) {
        LOGCATE("FBOSample::Init CreateFrameBufferObj fail");
        return;
    }
}

bool FBOSample::CreateFrameBufferObj() {
    LOGCATD("FBOSample ::CreateFrameBufferObj  start");
    glGenTextures(1, &m_FboTextureId);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);


    glGenFramebuffers(1,&m_FboId);
    glBindFramebuffer(GL_FRAMEBUFFER,m_FboId);
    glBindTexture(GL_TEXTURE_2D,m_FboTextureId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FboTextureId, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGCATE("FBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
        return false;
    }
    // 解绑纹理，
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    LOGCATD("FBOSample ::CreateFrameBufferObj  end");
    return true;

}

void FBOSample::Draw(int screenW, int screenH) {
    LOGCATD("FBOSample ::Draw  start");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glViewport(0, 0, m_RenderImage.width, m_RenderImage.height);

    // start off-screen render
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
    glUseProgram(m_FboProgramObj);
    // 设置当前VAO，之后所有操作
    // (注意：这些操作必须是上文VAO中包含的内容所注明的调用，
    // 其他非VAO中存储的内容即使调用了也不会影响VAO）存储在该VAO中
    glBindVertexArray(m_VaoIds[1]);
    glActiveTexture(GL_TEXTURE0);
    // m_ImageTextureId 不用区分十分使用了离屏渲染
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
    glUniform1i(m_FboSamplerLoc, 0);

    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    // 解绑VAO（解绑主要是为了不影响后续VAO的设置，有点类似于C++中指针delete后置空，是个好习惯）
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // 普通渲染
    glViewport(0, 0, screenW, screenH);
    glUseProgram(m_ProgramObj);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(m_VaoIds[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glUniform1i(m_SampleLoc, 0);
    GO_CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);
    GO_CHECK_GL_ERROR();
    // unbind.
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
    LOGCATD("FBOSample ::Draw  End");
}

void FBOSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
    }

    if (m_FboProgramObj) {
        glDeleteProgram(m_FboProgramObj);
    }
    if (m_ImageTextureId) {
        glDeleteTextures(1, &m_ImageTextureId);
    }
    if (m_ImageTextureId) {
        glDeleteTextures(1, &m_ImageTextureId);
    }

    if (m_FboTextureId) {
        glDeleteTextures(1, &m_FboTextureId);
    }
    if (m_VboIds[0]) {
        glDeleteBuffers(4, m_VboIds);
    }

    if (m_VaoIds[0]) {
        glDeleteVertexArrays(2, m_VaoIds);
    }
    if (m_FboId) {
        glDeleteFramebuffers(1, &m_FboId);
    }

}

FBOSample::~FBOSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);

}
