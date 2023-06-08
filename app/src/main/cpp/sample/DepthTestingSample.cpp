//
// Created by dev on 2023/6/7.
//
#include <gtc/matrix_transform.hpp>
#include "DepthTestingSample.h"
#include "GLUtils.h"


// 13x3 的矩阵
glm::vec3 transPos[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 2.0f, -1.0f) * 1.6f,
        glm::vec3(-1.5f, -2.2f, -1.5f) * 1.6f,
        glm::vec3(-1.8f, -2.0f, 1.3f) * 1.6f,
        glm::vec3(1.4f, -1.4f, -1.5f) * 1.6f,
        glm::vec3(-1.7f, 2.0f, -1.5f) * 1.6f,
        glm::vec3(1.3f, -2.0f, 2.5f) * 1.6f,
        glm::vec3(0.5f, 1.3f, -0.1f) * 1.6f,
        glm::vec3(1.5f, 2.2f, 1.5f) * 1.6f,
        glm::vec3(-1.3f, 1.0f, -1.5f) * 1.6f,
        glm::vec3(-1.3f, 0.0f, -1.5f) * 1.6f,
        glm::vec3(0.0f, -1.3f, -0.5f) * 1.6f,
        glm::vec3(0.0f, -1.5f, 1.5f) * 1.6f,
};

DepthTestingSample::DepthTestingSample() {

    m_SamplerLoc = GL_NONE;
    m_MVPMatLoc  = GL_NONE;
    m_TextureId  = GL_NONE;
    m_VaoId      = GL_NONE;

    m_AngleX      = 0;
    m_AngleY      = 0;
    m_ModelMatrix = glm::mat4(0.0f);

}

DepthTestingSample::~DepthTestingSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);

}

void DepthTestingSample::LoadImage(NativeImage *pImage) {
    if (pImage) {
        m_RenderImage.width  = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::copyNativeImage(pImage, &m_RenderImage);
    }
}

void DepthTestingSample::Init() {
    if (m_ProgramObj) {
        return;
    }

    // create RGBA texture ;
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    char vShaderStr[] =
                 "#version 300 es\n"
                 "precision mediump float;\n"
                 "layout(location = 0) in vec4 a_position;\n"
                 "layout(location = 1) in vec2 a_texCoord;\n"
                 "layout(location = 2) in vec3 a_normal;\n"
                 "\n"
                 "out vec2 v_texCoord;\n"
                 "uniform mat4 u_MVPMatrix;\n"
                 "void main()\n"
                 "{\n"
                 "    gl_Position = u_MVPMatrix * a_position;\n"
                 "    v_texCoord = a_texCoord;\n"
                 "}";

    char fShaderStr[] =
                 "#version 300 es\n"
                 "precision mediump float;\n"
                 "in vec2 v_texCoord;\n"
                 "layout(location = 0) out vec4 outColor;\n"
                 "uniform sampler2D s_TextureMap;\n"
                 "void main()\n"
                 "{\n"
                 "    vec4 objectColor = vec4(vec3(gl_FragCoord.z), 1.0f);\n"
                 "    outColor = objectColor;\n"
                 "}";
    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);


    if (m_ProgramObj) {
        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "m_SamplerLoc");
        m_MVPMatLoc  = glGetUniformLocation(m_ProgramObj, "u_MVPMatrix");
    } else {
        LOGCATE("DepthTestingSample::Init create program fail");
        return;
    }
    GLfloat vertices[] = {
            //position            //texture coord  //normal
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    };

    // 给VBO 设置数据
    glGenBuffers(1, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 生成vao
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);
    // 解析VAO数据
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (const void *) (3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (const void *) (5 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    // 根据相关参数，生成一个2d纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

}

void DepthTestingSample::Draw(int screenW, int screenH) {
    if (m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) {
        return;
    }
    // 不清除上一次的 深度缓冲区的值 ，可能会导致黑屏；
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(
            1.0f,
            1.0f,
            1.0f,
            1.0f
                );
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    // 另外在一些场景中，我们需要进行深度测试并相应地丢弃片段，
    // 但我们不希望更新深度缓冲区，那么可以设置深度掩码glDepthMask(GL_FALSE);
    // 实现禁用深度缓冲区的写入（只有在深度测试开启时才有效）。
    glDepthMask(GL_FALSE);


    auto ratio = static_cast<float>((float) screenW / (float) screenH);
    glUseProgram(m_ProgramObj);

    glBindVertexArray(m_VaoId);

    // 激活纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glUniform1i(m_SamplerLoc, 0);

    //绘制多个正方体
    for (const auto &item: transPos) {
        UpdateMatrix(m_MVPMatrix, m_ModelMatrix, m_AngleX + 10, m_AngleY + 10, 0.4, item, ratio);
        glUniformMatrix4fv(m_MVPMatLoc, 1, GL_FALSE, &m_MVPMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}

void DepthTestingSample::Destroy() {

    if (!m_ProgramObj) {
        return;
    }
    glDeleteProgram(m_ProgramObj);
    glDeleteBuffers(1, m_VboIds);
    glDeleteBuffers(1, &m_VaoId);
    glDeleteTextures(1, &m_TextureId);
    m_ProgramObj = GL_NONE;
    m_VaoId      = GL_NONE;
    m_TextureId  = GL_NONE;


}

void DepthTestingSample::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX,
                                               float scaleY) {
    GLSampleBase::UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    m_AngleX = static_cast<int >(rotateX);
    m_AngleY = static_cast<int >(rotateY);
}

void
DepthTestingSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float scaleY) {

}

void
DepthTestingSample::UpdateMatrix(glm::mat4 &mvpMatrix, glm::mat4 &modelMatrix, int angleXRotate,
                                 int angleYRotate, float scale, glm::vec3 transVec3, float ratio) {
    angleXRotate %= 360;
    angleYRotate %= 360;
    // 转换为 弧度角
    auto      radianX    = static_cast<float>(MATH_PI / 180.0f * angleXRotate);
    auto      radianY    = static_cast<float>(MATH_PI / 180.0f * angleYRotate);
    // step 1: projection matrix
    glm::mat4 Projection = glm::perspective(
            45.0f, // 可见的范围
            ratio, // 屏幕宽高比
            0.1f, // z 轴近
            100.f // z 轴远
                                           );
    // step 2: view matrix
    glm::mat4 View       = glm::lookAt(
            glm::vec3(0, 0, 3), // 世界坐标中，camera的位置
            glm::vec3(0, 0, 0), //
            glm::vec3(0, 1, 0)//
                                      );

    // step 3: Model matrix
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(scale, scale, scale));
    Model = glm::rotate(Model, radianX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, radianY, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, transVec3);

    modelMatrix = Model;
    mvpMatrix   = Projection * View * Model;


}
