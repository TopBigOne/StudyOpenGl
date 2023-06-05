//
// Created by dev on 2023/6/5.
//
#include <gtc/matrix_transform.hpp>

#include "CoordSystemSample.h"
#include "../util/GLUtils.h"


CoordSystemSample::CoordSystemSample() {
    m_SamplerLoc = GL_NONE;
    m_MVPMatLoc  = GL_NONE;
    m_TextureId  = GL_NONE;
    m_VaoId      = GL_NONE;

    m_AngleX = 0;
    m_AngleY = 0;

    m_ScaleX = 1.0f;
    m_ScaleY = 1.0f;
}

CoordSystemSample::~CoordSystemSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void CoordSystemSample::LoadImage(NativeImage *pImage) {
    if (pImage == nullptr) {
        return;
    }
    m_RenderImage.width  = pImage->width;
    m_RenderImage.height = pImage->height;
    m_RenderImage.format = pImage->format;
    NativeImageUtil::copyNativeImage(pImage, &m_RenderImage);
}

void CoordSystemSample::Init() {
    if (m_ProgramObj) {
        return;
    }
    // create a RGBA texture
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    // a_texCoord 是纹理坐标
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
                 "#version 300 es                                     \n"
                 "precision mediump float;                            \n"
                 "in vec2 v_texCoord;                                 \n"
                 "layout(location = 0) out vec4 outColor;             \n"
                 "uniform sampler2D s_TextureMap;                     \n"
                 "void main()                                         \n"
                 "{                                                   \n"
                 "  outColor = texture(s_TextureMap, v_texCoord);     \n"
                 "}                                                   \n";

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    if (m_ProgramObj) {
        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
        m_MVPMatLoc  = glGetUniformLocation(m_ProgramObj, "u_MVPMatrix");
    } else {
        LOGCATE("CoordSystemSample::Init create program fail");
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
            1.0f, 1.0f,        // TexCoord 2
            1.0f, 0.0f         // TexCoord 3
    };

    // VBO content
    GLushort indices[] = {0, 1, 2, 0, 2, 3};

    glGenBuffers(3, m_VboIds);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

    // vbo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // generate vao Id
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    // veo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);


    glBindVertexArray(GL_NONE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);


}

void CoordSystemSample::Draw(int screenW, int screenH) {
    LOGCATI("CoordSystemSample::Draw()");
    if (m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) {
        return;
    }

    UpdateMVPMatrix(m_MVPMatrix, m_AngleX, m_AngleY, (float) screenW / (float) screenH);
    // use the program object.
    glUseProgram(m_ProgramObj);
    glBindVertexArray(m_VaoId);
    glUniformMatrix4fv(m_MVPMatLoc, 1, GL_FALSE, &m_MVPMatrix[0][0]);

    // Bind the RGBA map;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glUniform1i(m_SamplerLoc, 0);

    // 因为 indices 是SHORT 类型
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);

}

void CoordSystemSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        glDeleteBuffers(3, m_VboIds);
        glDeleteVertexArrays(1, &m_VaoId);
        glDeleteTextures(1, &m_TextureId);
    }

}


void
CoordSystemSample::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {
    GLSampleBase::UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    m_AngleX = static_cast<int>(rotateX);
    m_AngleY = static_cast<int>(rotateY);
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;

}


/**
 *
 * @param rotateX   绕X轴旋转度数
 * @param rotateY   绕Y轴旋转度数
 * @param scaleX
 * @param scaleY
 */
void
CoordSystemSample::UpdateMVPMatrix(glm::mat4 &mvpMartrix, int angleX, int angleY, float ratio) {
    LOGCATI("CoordSystemSample::UpdateMVPMatrix angleX = %d, angleY = %d, ratio = %f", angleX,
            angleY, ratio);
    angleX = angleX % 360;
    angleY = angleY % 360;
    // 转化为弧度角
    auto      radianX    = static_cast<float>(MATH_PI / 180.f * angleX);
    auto      radianY    = static_cast<float>(MATH_PI / 180.f * angleY);
    // 透视投影矩阵
    glm::mat4 ProjectionMat4 = glm::perspective(45.0f, ratio, 0.1f, 100.f);

    // view matrix
    glm::mat4 ViewMat4  = glm::lookAt(
            glm::vec3(0, 0, 4),// camera is at(0,0,1) in World Space.
            glm::vec3(0, 0, 0),//  and looks at origin
            glm::vec3(0, 1, 0)//  and looks at origin
                                 );
    glm::mat4 ModelMat4 = glm::mat4(1.0f);
    ModelMat4  = glm::scale(ModelMat4, glm::vec3(m_ScaleX, m_ScaleY, 1.0f));
    ModelMat4  = glm::rotate(ModelMat4, radianX, glm::vec3(1.0f, 1.0f, 0.0f));
    ModelMat4  = glm::rotate(ModelMat4, radianY, glm::vec3(0.0f, 1.0f, 0.0f));
    ModelMat4  = glm::translate(ModelMat4, glm::vec3(0.0f, 0.0f, 0.0f));
    mvpMartrix = ProjectionMat4 * ViewMat4 * ModelMat4;


}
