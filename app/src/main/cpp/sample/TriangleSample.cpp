//
// Created by dev on 2023/5/25.
//

#include "TriangleSample.h"
#include "../util/LogUtils.h"
#include "../util/GLUtils.h"

TriangleSample::TriangleSample() {


}

TriangleSample::~TriangleSample() {

}

void TriangleSample::LoadImage(NativeImage *pImage) {


}

void TriangleSample::Init() {
    if (m_ProgramObj != 0) {
        return;
    }

    char vShaderStr[] =
                 "#version 300 es                          \n"
                 "layout(location = 2) in vec4 vPosition;  \n"
                 "void main()                              \n"
                 "{                                        \n"
                 "   gl_Position = vPosition;              \n"
                 "}                                        \n";

    char fShaderStr[] =
                 "#version 300 es                              \n"
                 "precision mediump float;                     \n"
                 "out vec4 fragColor;                          \n"
                 "void main()                                  \n"
                 "{                                            \n"
                 "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
                 "}                                            \n";

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);


}

void TriangleSample::Draw(int screenW, int screenH) {
    // 顶点坐标
    GLfloat vVertices[] = {
            0.0f, 0.5f, 0.0,
            -0.5f, -0.5f, 0.0,
            0.5f, -0.5f, 0.0f
    };

    GLfloat vVertices1[] = {
            0.0f, 0.3f, -0.9f,
            -0.3f, -0.3f, -0.9f,
            0.3f, -0.3f, 1.0f
    };

    glm::vec3 myVertices[] = {
            glm::vec3(0.0f, 0.5f, 0.0),
            glm::vec3(-0.5f, -0.5f, 0.0),
            glm::vec3(0.5f, -0.5f, 0.0f)
    };
    glm::vec3 scale(2, 2, 0);
    for (auto &myVerticse: myVertices) {
        myVerticse *= scale;
    }

    if (m_ProgramObj == 0) {
        return;
    }

    int vPositionIndex = 2;
    glUseProgram(m_ProgramObj);
    glVertexAttribPointer(vPositionIndex, 3, GL_FLOAT, GL_FALSE, 0, myVertices);
    glEnableVertexAttribArray(vPositionIndex);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(GL_NONE);

}

void TriangleSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        m_ProgramObj = GL_NONE;
    }


}
