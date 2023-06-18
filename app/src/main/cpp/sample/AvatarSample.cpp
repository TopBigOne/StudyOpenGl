//
// Created by dev on 2023/6/17.
//

#include <gtc/matrix_transform.hpp>
#include "GLUtils.h"
#include "AvatarSample.h"

float clamp(float x, float lowerLimit, float upperLimit) {
    if (x < lowerLimit)
        x = lowerLimit;
    if (x > upperLimit)
        x = upperLimit;
    return x;
}

float smoothStep(float edge0, float edge1, float x) {
    x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return x * x * (3 - 2 * x);
}


AvatarSample::AvatarSample() {

    m_SamplerLoc = GL_NONE;
    m_MVPMatLoc  = GL_NONE;

    m_VaoId = GL_NONE;

    m_AngleX = 0;
    m_AngleY = 0;

    m_ScaleX = 1.0f;
    m_ScaleY = 1.0f;

    m_TransX = 0.0f;
    m_TransY = 0.0f;

    // 全部设置为0
    memset(m_TextureIds, 0, sizeof(m_TextureIds));
    memset(m_RenderImages, 0, sizeof(m_RenderImages));

    m_FrameIndex = 0;
}


AvatarSample::~AvatarSample() {
    for (int i = 0; i < RENDER_IMG_NUM; ++i) {
        NativeImageUtil::FreeNativeImage(&m_RenderImages[i]);
    }
}

void AvatarSample::Init() {
    if (m_ProgramObj) {
        return;
    }
    glGenTextures(RENDER_IMG_NUM, m_TextureIds);

    for (int i = 0; i < RENDER_IMG_NUM; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_TextureIds[i]);
        // 设置2D 纹理数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImages[i].width, m_RenderImages[i].height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, m_RenderImages[i].ppPlane[0]);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }


    char vShaderStr[] =
                 "#version 300 es\n"
                 "layout(location = 0) in vec4 a_position;\n"
                 "layout(location = 1) in vec2 a_texCoord;\n"
                 "uniform mat4 u_MVPMatrix;\n"
                 "out vec2 v_texCoord;\n"
                 "void main()\n"
                 "{\n"
                 "    gl_Position = u_MVPMatrix * a_position;\n"
                 "    v_texCoord = a_texCoord;\n"
                 "}";

    char fShaderStr[] =
                 "#version 300 es\n"
                 "precision highp float;\n"
                 "in vec2 v_texCoord;\n"
                 "layout(location = 0) out vec4 outColor;\n"
                 "uniform sampler2D s_TextureMap;\n"
                 "uniform vec2 u_texSize;\n"
                 "uniform float u_needRotate;\n"
                 "uniform float u_rotateAngle;\n"
                 "\n"
                 "vec2 rotate(float radius, float angle, vec2 texSize, vec2 texCoord)\n"
                 "{\n"
                 "    vec2 newTexCoord = texCoord;\n"
                 "    vec2 center = vec2(texSize.x / 2.0, texSize.y / 2.0);\n"
                 "    vec2 tc = texCoord * texSize;\n"
                 "    tc -= center;\n"
                 "    float dist = length(tc);\n"
                 "    if (dist < radius) {\n"
                 "        float percent = (radius - dist) / radius;\n"
                 "        float theta = percent * percent * angle * 8.0;\n"
                 "        float s = sin(theta);\n"
                 "        float c = cos(theta);\n"
                 "        tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));\n"
                 "        tc += center;\n"
                 "\n"
                 "        newTexCoord = tc / texSize;\n"
                 "    }\n"
                 "    return newTexCoord;\n"
                 "}\n"
                 "void main()\n"
                 "{\n"
                 "    vec2 texCoord = v_texCoord;\n"
                 "\n"
                 "    if(u_needRotate > 0.0)\n"
                 "    {\n"
                 "        texCoord = rotate(0.5 * u_texSize.x, u_rotateAngle, u_texSize, v_texCoord);\n"
                 "    }\n"
                 "\n"
                 "    outColor = texture(s_TextureMap, texCoord);\n"
                 "    if (outColor.a < 0.6) discard;\n"
                 "}";

    char fBlurShaderStr[] =
                 "#version 300 es\n"
                 "precision highp float;\n"
                 "layout(location = 0) out vec4 outColor;\n"
                 "in vec2 v_texCoord;\n"
                 "uniform sampler2D s_TextureMap;\n"
                 "void main() {\n"
                 "    vec4 sample0, sample1, sample2, sample3;\n"
                 "    float blurStep = 0.16;\n"
                 "    float step = blurStep / 100.0;\n"
                 "    sample0 = texture(s_TextureMap, vec2(v_texCoord.x - step, v_texCoord.y - step));\n"
                 "    sample1 = texture(s_TextureMap, vec2(v_texCoord.x + step, v_texCoord.y + step));\n"
                 "    sample2 = texture(s_TextureMap, vec2(v_texCoord.x + step, v_texCoord.y - step));\n"
                 "    sample3 = texture(s_TextureMap, vec2(v_texCoord.x - step, v_texCoord.y + step));\n"
                 "    outColor = (sample0 + sample1 + sample2 + sample3) / 4.0;\n"
                 "    if(outColor.a > 0.05) \n"
                 "    {\n"
                 "        outColor += vec4(0.2, 0.2, 0.2, 0.0);\n"
                 "    } \n"
                 "    else\n"
                 "    {\n"
                 "        discard;\n"
                 "    }\n"
                 "}";

    m_ProgramObj     = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader,
                                              m_FragmentShader);
    m_BlurProgramObj = GLUtils::CreateProgram(vShaderStr, fBlurShaderStr);
    if (m_ProgramObj) {
        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
        m_MVPMatLoc  = glGetUniformLocation(m_ProgramObj, "u_MVPMatrix");
    } else {
        LOGCATE("AvatarSample::Init create program fail");
    }

    GLfloat verticesCoords[] = {
            -1.0f, 1.0f, 0.0f,  // Position 0
            -1.0f, -1.0f, 0.0f,  // Position 1
            1.0f, -1.0f, 0.0f,  // Position 2
            1.0f, 1.0f, 0.0f,  // Position 3
    };

    GLfloat textureCoords[] = {
            0.0f, 0.0f,        // TexCoord 0
            0.0f, 1.0f,        // TexCoord 1
            1.0f, 1.0f,        // TexCoo	rd 2
            1.0f, 0.0f         // TexCoord 3
    };

    GLushort indices[] = {0, 1, 2, 0, 2, 3};

    glGenBuffers(3, m_VboIds);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);


    glBindVertexArray(GL_NONE);

}


void AvatarSample::LoadImage(NativeImage *pImage) {
    LOGCATE("AvatarSample::LoadImage pImage = %p", pImage->ppPlane[0]);

}

void AvatarSample::LoadMultiImageWithIndex(int index, NativeImage *pImage) {
    LOGCATE("AvatarSample::LoadMultiImageWithIndex pImage = %p, index = %d", pImage, index);
    if (pImage->ppPlane[0]) {
        m_RenderImages[index].width  = pImage->width;
        m_RenderImages[index].height = pImage->height;
        m_RenderImages[index].format = pImage->format;
        NativeImageUtil::copyNativeImage(pImage, &m_RenderImages[index]);
    }
}

void AvatarSample::Draw(int screenW, int screenH) {
    if (m_ProgramObj == GL_NONE) {
        return;
    }
    float dScaleLevel = m_FrameIndex % 200 * 1.0f / 1000 + 0.0001f;
    float scaleLevel  = 1.0;
    auto  ratio       = static_cast<float >(screenW / screenH);

    glClearColor(1.0f,
                 1.0f,
                 1.0f,
                 1.0f);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // install program object.
    glUseProgram(m_ProgramObj);
    // bind vertex.
    glBindVertexArray(m_VaoId);

    // select a texture unit to active.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureIds[0]);
    glUniform1i(m_SamplerLoc, 0);
    scaleLevel = static_cast<float>(1.0f + dScaleLevel * pow(-1, m_FrameIndex / 200));
    scaleLevel = scaleLevel < 1.0 ? scaleLevel + 0.2f : scaleLevel;
    m_ScaleY   = m_ScaleX = scaleLevel + 0.4f;
    GLUtils::setVec2(m_ProgramObj, "u_texSize",
                     glm::vec2(m_RenderImages[0].width, m_RenderImages[0].height));
    //  u_needRotate == 1 开启形变,,,,TMD 其实是这个布尔值
    GLUtils::setFloat(m_ProgramObj, "u_needRotate", 1.0f);
    GLUtils::setFloat(m_ProgramObj, "u_rotateAngle", m_TransX * 1.5f);
    UpdateMVPMatrix(m_MVPMatrix, m_AngleX, m_AngleY, m_TransX / 2, m_TransY / 2,
                    ratio);
    glUniformMatrix4fv(m_MVPMatLoc, 1, GL_FALSE, &m_MVPMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    // 2: 绘制阿凡达人像
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_TextureIds[1]);
    // 将纹理对象（数据）从CPU中传入显存中的着色器
    // 之所以使用glUniform1i（）函数，是因为只需要给纹理采样器传入一个索引值(位置)即可，
    // 这样我们就能够在一个片元着色器中设置多个纹理。
    glUniform1i(m_SamplerLoc, 1);
    scaleLevel = static_cast<float>(1.0f + dScaleLevel * pow(-1, m_FrameIndex / 200 + 1));
    scaleLevel = scaleLevel < 1.0 ? scaleLevel + 0.2f : scaleLevel;
    m_ScaleY   = m_ScaleX = scaleLevel + 0.4f;
    UpdateMVPMatrix(m_MVPMatrix, m_AngleX, m_AngleY, m_TransX * 1.2f, m_TransY * 1.2f, ratio);
    GLUtils::setVec2(m_ProgramObj, "u_texSize",
                     glm::vec2(m_RenderImages[0].width, m_RenderImages[0].height));
    GLUtils::setFloat(m_ProgramObj, "u_needRotate", 0.0f);// u_needRotate == 0 关闭形变
    GLUtils::setFloat(m_ProgramObj, "u_rotateAngle", m_TransX / 20);
    glUniformMatrix4fv(m_MVPMatLoc, 1, GL_FALSE, &m_MVPMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    // 3: 外层的白色星星
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(m_BlurProgramObj);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureIds[2]);

    GLUtils::setFloat(m_BlurProgramObj, "s_TextureMap", 0);
    // -1 的 (m_FrameIndex/200) 次方。
    scaleLevel = static_cast<float >(dScaleLevel * pow(-1, m_FrameIndex / 200) + 1.0f);
    scaleLevel = scaleLevel < 1.0 ? scaleLevel + 0.2f : scaleLevel;
    m_ScaleY = m_ScaleX = scaleLevel +0.8f;
    UpdateMVPMatrix(m_MVPMatrix,m_AngleX,m_AngleY,m_TransX*2.5f,m_TransY*2.5,ratio);
    GLUtils::setMat4(m_BlurProgramObj, "u_MVPMatrix", m_MVPMatrix);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);


    glDisable(GL_BLEND);
    m_FrameIndex++;
}

void AvatarSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        glDeleteBuffers(3, m_VboIds);
        glDeleteVertexArrays(1, &m_VaoId);
        glDeleteTextures(RENDER_IMG_NUM, m_TextureIds);
    }
}


/**
 * 更新 MVP
 * @param mvpMatrix
 * @param angleX 绕X轴旋转度数
 * @param angleY 绕Y轴旋转度数
 * @param transX 沿X轴位移大小
 * @param transY 沿Y轴位移大小
 * @param ratio 宽高比
 */
void AvatarSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float transX,
                                   float transY, float ratio) {
    angleX %= 360;
    angleY %= 360;

    auto      radiansX   = static_cast<float>(MATH_PI / 180.0f * angleX);
    auto      radiansY   = static_cast<float>(MATH_PI / 180.0f * angleY);
    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 View       = glm::lookAt(
            glm::vec3(0, 0, 4),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0));
    glm::mat4 Model      = glm::mat4(1.0f);
    Model     = glm::scale(Model, glm::vec3(m_ScaleX, m_ScaleY, 1.0f));
    Model     = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0, 0));
    Model     = glm::rotate(Model, radiansY, glm::vec3(0, 1.0f, 0));
    Model     = glm::translate(Model, glm::vec3(transX, transY, 0));
    mvpMatrix = Projection * View * Model;
}

void AvatarSample::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {
    GLSampleBase::UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    m_AngleX = static_cast<int>(rotateX);
    m_AngleY = static_cast<int>(rotateY);
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
}

void AvatarSample::SetGravityXY(float x, float y) {
    m_TransX = -x / 60;
    m_TransX = -x / 60;
}



