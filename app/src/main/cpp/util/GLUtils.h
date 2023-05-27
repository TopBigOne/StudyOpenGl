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


};


#endif //STUDYOPENGL3_0_GLUTILS_H
