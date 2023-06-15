//
// Created by dev on 2023/6/10.
//

#include "GLUtils.h"
#include "LogUtils.h"
#include <vec2.hpp>
#include "InstancingSample.h"


InstancingSample::InstancingSample() {
    LOGCATI("InstancingSample::InstancingSample()");
    m_InstanceVbo = GL_NONE;
    m_VerticesVbo = GL_NONE;
    m_Vao         = GL_NONE;

}

InstancingSample::~InstancingSample() {

}

void InstancingSample::Init() {
    LOGCATI("InstancingSample::Init()");

    /**
     *   gl_InstanceID ： 内置变量：从右上到左下逐渐缩小四边形
     */
    char vShaderStr[] =
                 "#version 300 es                                 \n"
                 "layout(location = 0) in vec2 vPosition;         \n"
                 "layout(location = 1) in vec3 color;             \n"
                 "layout(location = 2) in vec2 offset;            \n"
                 "out vec3 fColor;                                \n"
                 "void main()                                     \n"
                 "{                                               \n"
                 "   float instanceID = float(gl_InstanceID);     \n"
                 "   vec2 pos = vPosition * (instanceID / 100.0); \n"
                 "   gl_Position = vec4(pos + offset, 0.0, 1.0);  \n"
                 "   fColor = color;                              \n"
                 "}                                               \n";

    /**
     *  没啥，，就是片段着色器
     */
    char fShaderStr[] =
                 "#version 300 es                              \n"
                 "precision mediump float;                     \n"
                 "out vec4 fragColor;                          \n"
                 "in  vec3 fColor;                             \n"
                 "void main()                                  \n"
                 "{                                            \n"
                 "   fragColor = vec4 (fColor, 1.0);           \n"
                 "}                                            \n";

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);


    /**
     * 每个四边形由2个三角形所组成，一共有6个顶点。
     * 每个顶点包含一个2D的标准化设备坐标位置向量和一个颜色向量
     */
    GLfloat   vVertices[] = {
            // Positions               // Colors :RGB
            -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
            -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

            -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
            0.05f, 0.05f, 0.0f, 1.0f, 1.0f
    };
    glm::vec2 translations[100];
    int       index       = 0;
    GLfloat   offset      = 0.1f;

    for (GLint y = -10; y < 10; y += 2) {
        for (GLint x = -10; x < 10; x += 2) {
            glm::vec2 translation;
            translation.x = (GLfloat) x / 10.0f + offset;
            translation.y = (GLfloat) y / 10.0f + offset;
            translations[index++] = translation;
        }
    }
    // instance VBO
    glGenBuffers(1, &m_InstanceVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVbo);
    // todo 研究一下指针数组
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glGenVertexArrays(1, &m_Vao);
    glGenBuffers(1, &m_VerticesVbo);

    // bind vertex -----start
    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_VerticesVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (GLvoid *) (2 * sizeof(GLfloat)));

    // offset :
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVbo);
    // size = 2 : 一次取在vec2 中2个数据，正好取完
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    //  Tell OpenGL this is an instanced vertex attribute.

    //  index   :  顶点属性的编号
    //  divisor : 增量因子，表示每个顶点属性的增量步长
    // 如果我们要在渲染100个实例时使用第0个顶点属性，并且希望每隔10个实例更新一次属性，则可以使用 : glVertexAttribDivisor(0, 10);
    glVertexAttribDivisor(2, 1);
    glBindVertexArray(GL_NONE);
    // bind vertex -----end






}

void InstancingSample::Draw(int screenW, int screenH) {
    if (m_ProgramObj == GL_NONE) {
        return;
    }
    LOGCATI("InstancingSample::Draw");

    glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_ProgramObj);
    glBindVertexArray(m_Vao);
    // 100 triangles of 6 vertices each
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
    glBindVertexArray(GL_NONE);


}

void InstancingSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
    }


    if (m_InstanceVbo) {
        glDeleteBuffers(1, &m_InstanceVbo);
    }

    if (m_VerticesVbo) {
        glDeleteBuffers(1, &m_VerticesVbo);
    }

    if (m_Vao) {
        glDeleteVertexArrays(1, &m_Vao);
    }

}
