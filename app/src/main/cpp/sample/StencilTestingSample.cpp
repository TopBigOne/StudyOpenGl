//
// Created by dev on 2023/6/8.
//

#include <gtc/matrix_transform.hpp>
#include "StencilTestingSample.h"
#include "GLUtils.h"

StencilTestingSample::StencilTestingSample() {
    m_SampleLoc      = GL_NONE;
    m_MVPMatLoc      = GL_NONE;
    m_ModelMatrixLoc = GL_NONE;

    m_TextureId   = GL_NONE;
    m_VaoId       = GL_NONE;
    m_AngleX      = GL_NONE;
    m_AngleY      = GL_NONE;
    m_ModelMatrix = glm::mat4(0.0f);

}

StencilTestingSample::~StencilTestingSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void StencilTestingSample::LoadImage(NativeImage *pImage) {
    LOGCATI("StencilTestingSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage) {
        m_RenderImage.width  = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::copyNativeImage(pImage, &m_RenderImage);
    }
}

void StencilTestingSample::Init() {
    if (m_ProgramObj) {
        return;
    }
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
                 "out vec3 normal;\n"
                 "out vec3 fragPos;\n"
                 "out vec2 v_texCoord;\n"
                 "\n"
                 "uniform mat4 u_MVPMatrix;\n"
                 "uniform mat4 u_ModelMatrix;\n"
                 "\n"
                 "void main()\n"
                 "{\n"
                 "    gl_Position = u_MVPMatrix * a_position;\n"
                 "    fragPos = vec3(u_ModelMatrix * a_position);\n"
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
                 "    // Ambient\n"
                 "    float ambientStrength = 0.25;\n"
                 "    vec3 ambient = ambientStrength * light.color;\n"
                 "\n"
                 "    // Diffuse\n"
                 "    vec3 norm = normalize(normal);\n"
                 "    float diff = max(dot(norm, lightDir), 0.0);\n"
                 "    vec3 diffuse = diff * light.color;\n"
                 "\n"
                 "    // Specular\n"
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
                 "}";

    char fFrameShaderStr[] =
                 "#version 300 es\n"
                 "precision mediump float;\n"
                 "\n"
                 "layout(location = 0) out vec4 outColor;\n"
                 "\n"
                 "void main()\n"
                 "{\n"
                 "    outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                 "}";
    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    GLuint vertexShader, fragmentShader;
    m_FrameProgramObj = GLUtils::CreateProgram(vShaderStr, fFrameShaderStr, vertexShader,
                                               fragmentShader);

    if (m_ProgramObj && m_FrameProgramObj) {
        m_SampleLoc      = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
        m_MVPMatLoc      = glGetUniformLocation(m_ProgramObj, "u_MVPMatrix");
        m_ModelMatrixLoc = glGetUniformLocation(m_ProgramObj, "u_ModelMatrix");
        m_ViewPosLoc     = glGetUniformLocation(m_ProgramObj, "viewPos");

    } else {
        LOGCATE("StencilTestingSample::Init create program fail");
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

    glGenBuffers(1, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (const void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (const void *) (5 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);

    //upload RGBA image data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);


}

void StencilTestingSample::Draw(int screenW, int screenH) {
    if (m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) {
        return;
    }
    float ratio = (float) screenW / screenH;
    // step 1: 启动深度测试和模板测试，清空模板缓冲和深度缓冲
    glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    GO_CHECK_GL_ERROR()

    // step 2:  用 1 更新物体将被渲染的片段对应的模板值
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    GO_CHECK_GL_ERROR()
    // GL_KEEP    :保持现有的模板值
    // GL_REPLACE :将模板值设置为用 glStencilFunc 函数设置的ref值
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // //若模板测试和深度测试都通过了，将片段对应的模板值替换为1
    GO_CHECK_GL_ERROR()
    glStencilMask(0xFF);
    GO_CHECK_GL_ERROR()

    glBindVertexArray(m_VaoId);
    glUseProgram(m_ProgramObj);
    glUniform3f(m_ViewPosLoc, 0.0f, 0.0f, 3.0f);

    // bind the RGBA map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glUniform1i(m_SampleLoc, 0);
    // 设置光源的位置，颜色和方向
    glUniform3f(glGetUniformLocation(m_ProgramObj, "light.position"), 0.0f, 0.0f, 3.0f);
    glUniform3f(glGetUniformLocation(m_ProgramObj, "light.color"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(m_ProgramObj, "light.direction"),0.0f, 0.0f, -1.0f);

    // 用于计算边缘的过度，cutoff 表示内切光角，outerCutOff 表示外切光角
    glUniform1f(glGetUniformLocation(m_ProgramObj,"light.cutoff"),glm::cos(glm::radians(10.5f)));
    glUniform1f(glGetUniformLocation(m_ProgramObj,"light.outerCutoff"),glm::cos(glm::radians(11.5f)));

    // 衰减系数，常数项 constant 一次项 linear 和二次项 quadratic
    glUniform1f(glGetUniformLocation(m_ProgramObj,"light.constant"),1.0f);
    glUniform1f(glGetUniformLocation(m_ProgramObj,"light.linear"),0.09f);
    glUniform1f(glGetUniformLocation(m_ProgramObj,"light.quadratic"),0.032f);

    UpdateMatrix(m_MVPMatrix,m_ModelMatrix,m_AngleX,m_AngleY,1.0,glm::vec3(0.0f,0.0f,0.0f),ratio);
    glUniformMatrix4fv(m_MVPMatLoc,1,GL_FALSE,&m_MVPMatrix[0][0]);
    glUniformMatrix4fv(m_ModelMatrixLoc,1,GL_FALSE,&m_ModelMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES,0,36);
    GO_CHECK_GL_ERROR()

    glBindVertexArray(GL_NONE);

    // 当片段的模板值不为 1 时，片段通过测试进行渲染
    glStencilFunc(GL_NOTEQUAL,1,0xFF);
    //禁用模板写入和深度测试
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(m_VaoId);
    glUseProgram(m_FrameProgramObj);
    UpdateMatrix(m_MVPMatrix,m_ModelMatrix,m_AngleX,m_AngleY,1.05,glm::vec3(0.0f,0.0f,0.0f),ratio);
    GLUtils::setMat4(m_FrameProgramObj, "u_MVPMatrix", m_MVPMatrix);
    GLUtils::setMat4(m_FrameProgramObj,"u_ModelMatrix",m_ModelMatrix);
    glDrawArrays(GL_TRIANGLES,0,36);
    GO_CHECK_GL_ERROR();
    glBindVertexArray(GL_NONE);
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
}

void StencilTestingSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        glDeleteBuffers(1, m_VboIds);
        glDeleteVertexArrays(1, &m_VaoId);
        glDeleteTextures(1, &m_TextureId);
        m_ProgramObj = GL_NONE;
        m_VaoId      = GL_NONE;
        m_TextureId  = GL_NONE;
    }

}

void StencilTestingSample::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX,
                                                 float scaleY) {
    GLSampleBase::UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);

    m_AngleX = static_cast<int>(rotateX);
    m_AngleY = static_cast<int>(rotateY);
}

void
StencilTestingSample::UpdateMatrix(glm::mat4 &mvpMatrix, glm::mat4 &modelMatrix, int angleXRotate,
                                   int angleYRotate, float scale, glm::vec3 transVec3,
                                   float ratio) {
    angleXRotate %= 180;
    angleYRotate %= 180;

    float radiaX = static_cast<float>(MATH_PI / 180.0f * angleXRotate);
    float radiaY = static_cast<float>(MATH_PI / 180.0f * angleYRotate);

    // Projection matrix
    glm::mat4 Projection = glm::perspective(45.0f, ratio, 0.1f, 100.f);
    // View  matrix
    glm::mat4 View       = glm::lookAt(
            glm::vec3(0, 0, 3),// camera at (0,0,1) in world space,
            glm::vec3(0, 0, 0),//
            glm::vec3(0, 1, 0)//
                                      );

    // Mode matrix
    glm::mat4 Model = glm::mat4(1.0f);
    Model       = glm::scale(Model, glm::vec3(scale, scale, scale));
    Model       = glm::rotate(Model, radiaX, glm::vec3(scale, scale, scale));
    Model       = glm::rotate(Model, radiaY, glm::vec3(scale, scale, scale));
    Model       = glm::translate(Model, transVec3);
    modelMatrix = Model;
    mvpMatrix   = Projection * View * Model;


}
