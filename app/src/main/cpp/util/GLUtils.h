//
// Created by dev on 2023/5/26.
//

#include <GLES3/gl3.h>
#include <string>
#include <glm.hpp>

#ifndef STUDYOPENGL3_0_GLUTILS_H
#define STUDYOPENGL3_0_GLUTILS_H

#define MATH_PI 3.1415926535897932384626433832802


class GLUtils {
public:
    static GLuint LoadShader(GLenum shaderType, const char *pSource);

    static GLuint CreateProgram(const char *pVertexShaderSource, const char *pFragmentSource);

    static GLuint CreateProgram(const char *pVertexShaderSource,
                                const char *pFragmentSource,
                                GLuint &vertexShaderHandle,
                                GLuint &fragShaderHandle);


    static GLuint CreateProgramWithFeedback(const char *pVertexShaderSource, const char *pFragmentSource,
                                            GLuint &vertexShaderHandle, GLuint &fragShaderHandle,
                                            const GLchar **varying,
                                            int varyingCount);
    static void DeleteProgram(GLuint &program);
    static void CheckGLError(const char *pGLOperation);


    static void setMat4(GLuint programId, const std::string &name, const glm::mat4 &mat) {
        glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    static void setInt(GLuint programId, const std::string &name, int value) {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
    }

    static void setFloat(GLuint programId, const std::string &name, float value) {
        glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
    }

    static void setVec2(GLuint programId, const std::string &name, const glm::vec2 &value) {
        glUniform2fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
    }



};


#endif //STUDYOPENGL3_0_GLUTILS_H
