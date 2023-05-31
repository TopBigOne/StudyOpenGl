//
// Created by dev on 2023/5/30.
//

#include "VaoSample.h"
#include "../util/LogUtils.h"
#include "../util/GLUtils.h"

#define VERTEX_POS_SIZE   3    // x,y,z
#define VERTEX_COLOR_SIZE 4    // r,g,b,a

#define VERTEX_POS_INDEX 0    // shader layout location
#define VERTEX_COLOR_INDEX 1  // shader layout location

#define  BUFFER_COUNT 2

int gl_float_size = 0;
#define  VERTEX_STRIDE       (sizeof(GLfloat)*(VERTEX_POS_SIZE + VERTEX_COLOR_SIZE))


VaoSample::VaoSample() {
    LOGCATI("VaoSample : 使用VAO");
    m_VaoId = 0;
}


void VaoSample::Init() {
    const char vShaderStr[] =
                       "#version 300 es                            \n"
                       "layout(location = 0) in vec4 a_position;   \n"
                       "layout(location = 1) in vec4 a_color;      \n"
                       "out vec4 v_color;                          \n"
                       "out vec4 v_position;                       \n"
                       "void main()                                \n"
                       "{                                          \n"
                       "    v_color = a_color;                     \n"
                       "    gl_Position = a_position;              \n"
                       "    v_position = a_position;               \n"
                       "}";


    const char fShaderStr[] =
                       "#version 300 es                         \n"
                       "precision mediump float;\n"
                       "in vec4 v_color;\n"
                       "in vec4 v_position;\n"
                       "out vec4 o_fragColor;\n"
                       "void main()\n"
                       "{\n"
                       "    float n = 10.0;\n"
                       "    float span = 1.0 / n;\n"
                       "    int i = int((v_position.x + 0.5)/span);\n"
                       "    int j = int((v_position.y + 0.5)/span);\n"
                       "\n"
                       "    int grayColor = int(mod(float(i+j), 2.0));\n"
                       "    if(grayColor == 1)\n"
                       "    {                   \n"
                       "        float luminance = v_color.r*0.299 + v_color.g*0.587 + v_color.b*0.114;    \n"
                       "        o_fragColor = vec4(vec3(luminance), v_color.a);                           \n"
                       "    }           \n"
                       "    else            \n"
                       "    {\n"
                       "        o_fragColor = v_color;\n"
                       "    }\n"
                       "}";

    // 4 vertices , with (x,y,z) (r,g,b,a) per-vertex;
    // 就是一个顶点 包含了顶点坐标和 一个像素的颜色值
    // 4 vertices, with(x,y,z) ,(r, g, b, a) per-vertex

    GLfloat vertices[4 * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE)] =
                    {
                            -0.5f, 0.5f, 0.0f,       // v0
                            1.0f, 0.0f, 0.0f, 1.0f,  // c0
                            -0.5f, -0.5f, 0.0f,       // v1
                            0.0f, 1.0f, 0.0f, 1.0f,  // c1
                            0.5f, -0.5f, 0.0f,        // v2
                            0.0f, 0.0f, 1.0f, 1.0f,  // c2
                            0.5f, 0.5f, 0.0f,        // v3
                            0.5f, 1.0f, 1.0f, 1.0f,  // c3
                    };


    GLushort indices[6] = {0, 1, 2, 0, 2, 3};
    // program
    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);

    // 生成2个buffer
    glGenBuffers(BUFFER_COUNT, m_VboIds);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    // GL_STATIC_DRAW: 静态绘制
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[1]);


    glEnableVertexAttribArray(VERTEX_POS_INDEX);
    glEnableVertexAttribArray(VERTEX_COLOR_INDEX);


    GLsizei stride = (sizeof(GLfloat)) * (VERTEX_POS_SIZE * VERTEX_COLOR_SIZE);

    glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE,
                          GL_FLOAT, GL_FALSE, VERTEX_STRIDE,
                          (const void *) 0);

    glVertexAttribPointer(VERTEX_COLOR_INDEX, VERTEX_COLOR_SIZE,
                          GL_FLOAT, GL_FALSE, VERTEX_STRIDE,
                          (const void *) (VERTEX_POS_SIZE * sizeof(GLfloat)));

    //解绑
    glBindVertexArray(GL_NONE);
}

void VaoSample::LoadImage(NativeImage *pImage) {
    // nothing to do.
}

void VaoSample::Draw(int screenW, int screenH) {
    if (m_ProgramObj == 0) {
        return;
    }
    glUseProgram(m_ProgramObj);
    glBindVertexArray(m_VaoId);

    // draw it with VAO settings;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *) 0);

    // return to the default VAO
    glBindVertexArray(GL_NONE);


}


void VaoSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        glDeleteBuffers(BUFFER_COUNT, m_VboIds);
        glDeleteVertexArrays(1, &m_VaoId);
        m_ProgramObj = GL_NONE;
    }

}


VaoSample::~VaoSample() {

}
