//
// Created by dev on 2023/5/26.
//

#include "GLUtils.h"
#include "LogUtils.h"

GLuint GLUtils::LoadShader(GLenum shaderType, const char *pSource) {
    GLuint shader = 0;
    FUN_BEGIN_TIME("GLUtils::LoadShader");
        shader = glCreateShader(shaderType);
        if (shader) {
            glShaderSource(shader, 1, &pSource, nullptr);
            glCompileShader(shader);
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen) {

                    char *buf = (char *) malloc(size_t(infoLen));
                    if (buf) {
                        glGetShaderInfoLog(shader, infoLen, nullptr, buf);
                        LOGCATE("GLUtils::LoadShader Could not compile shader %d:\n%s\n",
                                shaderType, buf);
                        free(buf);
                    }
                    glDeleteShader(shader);
                    shader = 0;
                }
            }
        }
    FUN_END_TIME("GLUtils::LoadShader");
    return shader;
}

GLuint GLUtils::CreateProgram(const char *pVertexShaderSource, const char *pFragmentSource) {

    GLuint vertexShaderHandle;
    GLuint fragShaderHandle;

    return CreateProgram(pVertexShaderSource, pFragmentSource, vertexShaderHandle,
                         fragShaderHandle);
}

/**
 * 创建 program
 * @param pVertexShaderSource
 * @param pFragmentSource
 * @param vertexShaderHandle   引用
 * @param fragShaderHandle   引用
 * @return
 */
GLuint GLUtils::CreateProgram(const char *pVertexShaderSource, const char *pFragmentSource,
                              GLuint &vertexShaderHandle, GLuint &fragShaderHandle) {

    GLuint program = 0;
    FUN_BEGIN_TIME("GLUtils::CreateProgram");
        vertexShaderHandle = LoadShader(GL_VERTEX_SHADER, pVertexShaderSource);
        if (!vertexShaderHandle) {
            return program;
        }
        fragShaderHandle = LoadShader(GL_FRAGMENT_SHADER, pFragmentSource);
        if (!fragShaderHandle) {
            return program;
        }

        program = glCreateProgram();
        if (program) {
            glAttachShader(program, vertexShaderHandle);
            CheckGLError("glAttachShader");
            glAttachShader(program, fragShaderHandle);
            CheckGLError("glAttachShader");
            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

            glDetachShader(program, vertexShaderHandle);
            glDeleteShader(vertexShaderHandle);
            vertexShaderHandle = 0;

            glDetachShader(program, fragShaderHandle);
            glDeleteShader(fragShaderHandle);
            fragShaderHandle = 0;

            if (linkStatus != GL_TRUE) {
                GLint bufLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
                if (bufLength) {
                    char *buf = (char *) malloc((size_t) bufLength);
                    if (buf) {
                        glGetProgramInfoLog(program, bufLength, nullptr, buf);
                        LOGCATE("GLUtils::CreateProgram Could not link program:\n%s\n", buf);
                        free(buf);
                    }
                }
                glDeleteProgram(program);
                program = 0;
            }


        }


    FUN_END_TIME("GLUtils::LoadShader");
    LOGCATI("GLUtils::CreateProgram program = %d", program);
    return program;
}

GLuint
GLUtils::CreateProgramWithFeedback(const char *pVertexShaderSource, const char *pFragmentSource,
                                   GLuint &vertexShaderHandle, GLuint &fragShaderHandle,
                                   const GLchar **varying, int varyingCount) {
    GLuint program = 0;
    FUN_BEGIN_TIME("GLUtils::CreateProgramWithFeedback");
        vertexShaderHandle = LoadShader(GL_VERTEX_SHADER, pVertexShaderSource);
        if (!vertexShaderHandle) {
            return program;
        }

        fragShaderHandle = LoadShader(GL_VERTEX_SHADER, pFragmentSource);
        if (!fragShaderHandle) {
            return program;
        }
        program = glCreateProgram();
        if (program) {
            glAttachShader(program, vertexShaderHandle);
            CheckGLError("glAttachShader");
            glAttachShader(program, fragShaderHandle);
            CheckGLError("glAttachShader");
            // transform feedback
            // 把顶点着色器处理后的数据写回顶点缓冲区对象（Vertex Buffer Objects, VBOs），避免GPU、CPU之间往返拷贝数据，节省时间。
            glTransformFeedbackVaryings(program, varyingCount, varying, GL_INTERLEAVED_ATTRIBS);
            GO_CHECK_GL_ERROR();

            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

            glDetachShader(program, vertexShaderHandle);
            glDeleteShader(vertexShaderHandle);
            vertexShaderHandle = 0;

            glDetachShader(program, fragShaderHandle);
            glDeleteShader(fragShaderHandle);
            fragShaderHandle = 0;
            if (linkStatus != GL_TRUE) {
                GLint buf_length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &buf_length);
                if (buf_length) {
                    char *buf = (char *) malloc((size_t) buf_length);
                    if (buf) {
                        glGetProgramInfoLog(program, buf_length, nullptr, buf);
                        LOGCATE("GLUtils::CreateProgramWithFeedback Could not link program:\n%s\n",
                                buf);
                        free(buf);
                    }
                }
                glDeleteProgram(program);
                program = 0;
            }

        }


        glLinkProgram(program);


    FUN_END_TIME("GLUtils::CreateProgramWithFeedback");
    LOGCATE("GLUtils::CreateProgramWithFeedback program = %d", program);
    return program;


}

void GLUtils::DeleteProgram(GLuint &program) {
    if (program) {
        glUseProgram(0);
        glDeleteProgram(program);
        program = 0;
    }


}

void GLUtils::CheckGLError(const char *pGLOperation) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGCATE("GLUtils::CheckGLError GL Operation %s() glError (0x%x)\n", pGLOperation, error);
    }

}
