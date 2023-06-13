//
// Created by dev on 2023/6/11.
//

#include <gtc/matrix_transform.hpp>
#include "Instancing3DSample.h"
#include "GLUtils.h"
#include "LogUtils.h"

Instancing3DSample::Instancing3DSample() {

    m_SamplerLoc     = GL_NONE;
    m_MVPMatLoc      = GL_NONE;
    m_ModelMatrixLoc = GL_NONE;

    m_TextureId = GL_NONE;
    m_VaoId     = GL_NONE;

    m_AngleX = 0;
    m_AngleY = 0;

    m_ScaleX = 2.0f;
    m_ScaleY = 2.0f;

    m_ModelMatrix = glm::mat4(0.0f);

}

Instancing3DSample::~Instancing3DSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);

}

void Instancing3DSample::LoadImage(NativeImage *pImage) {
    LOGCATI("Instancing3DSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage) {
        m_RenderImage.width  = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::copyNativeImage(pImage, &m_RenderImage);
    }

}

void Instancing3DSample::Init() {
    if (m_ProgramObj) {
        return;
    }
    //create RGBA texture
    glGenTextures(1, &m_TextureId);
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
                 "layout(location = 3) in vec3 offset;\n"
                 "\n"
                 "out vec3 normal;\n"
                 "out vec3 fragPos;\n"
                 "out vec2 v_texCoord;\n"
                 "\n"
                 "uniform mat4 u_MVPMatrix;\n"
                 "uniform mat4 u_ModelMatrix;\n"
                 "\n"
                 "void main()\n"
                 "{\n"
                 "    gl_Position = u_MVPMatrix * (a_position + vec4(offset, 1.0));\n"
                 "    fragPos = vec3(u_ModelMatrix * (a_position + vec4(offset, 1.0)));\n"
                 "    normal = mat3(transpose(inverse(u_ModelMatrix))) * a_normal;\n"
                 "    v_texCoord = a_texCoord;\n"
                 "}";

    char fShaderStr[] =
                 "#version 300 es\n"
                 "precision mediump float;\n"
                 "struct Light {\n"
                 "    vec3 position;\n"
                 "    vec3 direction;\n"
                 "    vec3 color;\n"
                 "    float cutOff;\n"
                 "    float outerCutOff;\n"
                 "\n"
                 "    float constant;\n"
                 "    float linear;\n"
                 "    float quadratic;\n"
                 "};\n"
                 "\n"
                 "in vec3 normal;\n"
                 "in vec3 fragPos;\n"
                 "in vec2 v_texCoord;\n"
                 "\n"
                 "layout(location = 0) out vec4 outColor;\n"
                 "uniform sampler2D s_TextureMap;\n"
                 "\n"
                 "uniform vec3 viewPos;\n"
                 "uniform Light light;\n"
                 "\n"
                 "void main()\n"
                 "{\n"
                 "    vec4 objectColor = texture(s_TextureMap, v_texCoord);\n"
                 "\n"
                 "    vec3 lightDir = normalize(light.position - fragPos);\n"
                 "\n"
                 "    // Check if lighting is inside the spotlight cone\n"
                 "    float theta = dot(lightDir, normalize(-light.direction));\n"
                 "\n"
                 "    float epsilon = light.cutOff - light.outerCutOff;\n"
                 "    float intensity = clamp((theta - light.outerCutOff) / epsilon,0.0, 1.0);\n"
                 "\n"
                 "    // Ambient 环境光\n"
                 "    float ambientStrength = 0.4;\n"
                 "    vec3 ambient = ambientStrength * light.color;\n"
                 "\n"
                 "    // Diffuse 散射光\n"
                 "    vec3 norm = normalize(normal);\n"
                 "    float diff = max(dot(norm, lightDir), 0.0);\n"
                 "    vec3 diffuse = diff * light.color;\n"
                 "\n"
                 "    // Specular 镜面光\n"
                 "    vec3 viewDir = normalize(viewPos - fragPos);\n"
                 "    vec3 reflectDir = reflect(-lightDir, norm);\n"
                 "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);\n"
                 "    vec3 specular = spec * light.color;\n"
                 "\n"
                 "    // Attenuation\n"
                 "    float distance    = length(light.position - fragPos);\n"
                 "    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
                 "\n"
                 "    // ambient  *= attenuation;  // Also remove attenuation from ambient, because if we move too far, the light in spotlight would then be darker than outside (since outside spotlight we have ambient lighting).\n"
                 "    diffuse  *= attenuation;\n"
                 "    specular *= attenuation;\n"
                 "\n"
                 "    diffuse *= intensity;\n"
                 "    specular*= intensity;\n"
                 "\n"
                 "    vec3 finalColor = (ambient + diffuse + specular) * vec3(objectColor);\n"
                 "    outColor = vec4(finalColor, 1.0f);\n"
                 "    //outColor = objectColor;\n"
                 "}";
    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    if (m_ProgramObj) {
        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
        GO_CHECK_GL_ERROR();
        m_MVPMatLoc = glGetUniformLocation(m_ProgramObj, "u_MVPMatrix");
        GO_CHECK_GL_ERROR();
        m_ModelMatrixLoc = glGetUniformLocation(m_ProgramObj, "u_ModelMatrix");
        GO_CHECK_GL_ERROR();
        m_ViewPosLoc = glGetUniformLocation(m_ProgramObj, "viewPos");
        GO_CHECK_GL_ERROR();
    } else {
        LOGCATE("Instancing3DSample::Init create program fail");
        return;
    }

    GLfloat vertices[] = {
            //position            //texture coord  //normal
            -0.08f, -0.08f, -0.08f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.08f, -0.08f, -0.08f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.08f, 0.08f, -0.08f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            0.08f, 0.08f, -0.08f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.08f, 0.08f, -0.08f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.08f, -0.08f, -0.08f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

            -0.08f, -0.08f, 0.08f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.08f, -0.08f, 0.08f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.08f, 0.08f, 0.08f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.08f, 0.08f, 0.08f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.08f, 0.08f, 0.08f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.08f, -0.08f, 0.08f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            -0.08f, 0.08f, 0.08f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.08f, 0.08f, -0.08f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.08f, -0.08f, -0.08f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.08f, -0.08f, -0.08f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.08f, -0.08f, 0.08f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.08f, 0.08f, 0.08f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            0.08f, 0.08f, 0.08f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.08f, 0.08f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.08f, -0.08f, -0.08f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.08f, -0.08f, -0.08f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.08f, -0.08f, 0.08f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.08f, 0.08f, 0.08f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.08f, -0.08f, -0.08f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            0.08f, -0.08f, -0.08f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            0.08f, -0.08f, 0.08f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            0.08f, -0.08f, 0.08f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            -0.08f, -0.08f, 0.08f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            -0.08f, -0.08f, -0.08f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

            -0.08f, 0.08f, -0.08f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.08f, 0.08f, -0.08f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.08f, 0.08f, 0.08f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.08f, 0.08f, 0.08f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.08f, 0.08f, 0.08f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.08f, 0.08f, -0.08f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    };

    glm::vec3  translations[125];
    int        index   = 0;
    GLfloat    offset  = 0.2f;
    for (GLint y       = -10; y < 10; y += 4) {
        for (GLint x = -10; x < 10; x += 4) {
            for (GLint z = -10; z < 10; z += 4) {
                glm::vec3 translation;
                translation.x = (GLfloat) x / 10.0f + offset;
                translation.y = (GLfloat) y / 10.0f + offset;
                translation.z = (GLfloat) z / 10.0f + offset;
                translations[index++] = translation;
            }
        }
    }

    // step 1 : 往缓冲区里写数据
    glGenBuffers(2, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 125, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    // step 2: g告诉GPU 如何解析这些数据
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    //  layout(location = 0) in vec4 a_position;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void *) 0);

    // layout(location = 1) in vec2 a_texCoord;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (const void *) (3 * sizeof(GLfloat)));

    // layout(location = 2) in vec3 a_normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (const void *) (5 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    // "layout(location = 3) in vec3 offset;
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          (const void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    // 每隔一个修改下标为3 的顶点属性；
    glVertexAttribDivisor(3, 1);
    glBindVertexArray(GL_NONE);

    //upload RGBA image data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

}

void Instancing3DSample::Draw(int screenW, int screenH) {
    if (m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) {
        return;
    }
    glEnable(GL_DEPTH_TEST);
    float ratio = (float) screenW / screenH;

    glUseProgram(m_ProgramObj);

    glBindVertexArray(m_VaoId);
    glUniform3f(m_ViewPosLoc, 0.0f, 0.0f, 3.0f);

    //  bind the RGBA map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glUniform1i(m_SamplerLoc, 0);

    // 设置光源的位置、颜色和方向
    glUniform3f(getUniformLocation("light.position"), 0.0f, 0.0f, 3.0f);
    glUniform3f(getUniformLocation("light.color"), 1.0f, 1.0f, 1.0f);
    glUniform3f(getUniformLocation("light.direction"), 0.0f, 0.0f, -1.0f);

    // 用于计算边缘的过度，cutOff 表示内切光角，outerCutOff 表示外切光角
    glUniform1f(getUniformLocation("light.cutOff"), glm::cos(glm::radians(10.5f)));
    glUniform1f(getUniformLocation("light.outerCutOff"), glm::cos(glm::radians(11.5f)));

    // 衰减系数,常数项 constant，一次项 linear 和二次项 quadratic。
    glUniform1f(getUniformLocation("light.constant"), 1.0f);
    glUniform1f(getUniformLocation("light.linear"), 0.09);
    glUniform1f(getUniformLocation("light.quadratic"), 0.032);

    UpdateMatrix(m_MVPMatrix, m_ModelMatrix, m_AngleX + 10, m_AngleY + 10,
                 m_ScaleX > m_ScaleY ? m_ScaleY : m_ScaleX, glm::vec3(0.0f, 0.0f, 0.0f), ratio);

    // 把转换矩阵 传给GPU。
    glUniformMatrix4fv(m_MVPMatLoc, 1, GL_FALSE, &m_MVPMatrix[0][0]);
    // 参数含义：插槽位置，几个矩阵（单插槽可以多矩阵），CPU传到GPU时是否需要转置，矩阵地址(从定义看是第一个数据的位置)
    glUniformMatrix4fv(m_ModelMatrixLoc, 1, GL_FALSE, &m_ModelMatrix[0][0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 125);
    glBindVertexArray(GL_NONE);
}

void Instancing3DSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        glDeleteBuffers(2, m_VboIds);
        glDeleteVertexArrays(1, &m_VaoId);
        glDeleteTextures(1, &m_TextureId);
        m_ProgramObj = GL_NONE;
        m_VaoId      = GL_NONE;
        m_TextureId  = GL_NONE;
    }

}


void
Instancing3DSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio) {
// No implement
}

void
Instancing3DSample::UpdateMatrix(glm::mat4 &mvpMatrix, glm::mat4 &modelMatrix, int angleXRotate,
                                 int angleYRotate, float scale, glm::vec3 transVec3, float ratio) {
    angleXRotate %= 360;
    angleYRotate %= 360;

    auto radiansX = static_cast<float>(MATH_PI / 180.f * angleXRotate);
    auto radiansY = static_cast<float>(MATH_PI / 180.f * angleYRotate);

    // Projection matrix ： 在远平面和近平面之间，摄像机看的就保留，反之，就裁剪掉。
    glm::mat4 Projection = glm::perspective(45.0f, ratio, 0.1f, 100.0f);
    // View matrix ： 针对摄像机
    glm::mat4 View       = glm::lookAt(
            glm::vec3(0, 0, 3),
            glm::vec3(0, 0, 0), // target 就是 centre(0,0,0)
            glm::vec3(0, 1, 0));  // up 也是向上的。
    // Model matrix : 旋转，位移
    glm::mat4 Model      = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(scale, scale, scale));
    Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    //Model = glm::translate(Model, transVec3);

    modelMatrix = Model;

    mvpMatrix = Projection * View * Model;


}


void Instancing3DSample::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX,
                                               float scaleY) {
    GLSampleBase::UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    m_AngleX = static_cast<int>(rotateX);
    m_AngleY = static_cast<int>(rotateY);
    m_ScaleX = 2 * scaleX;
    m_ScaleY = 2 * scaleY;
}

GLint Instancing3DSample::getUniformLocation(const char *name) {
    if (m_ProgramObj == GL_NONE) {
        return GL_NONE;
    }
    return glGetUniformLocation(m_ProgramObj, name);
}
