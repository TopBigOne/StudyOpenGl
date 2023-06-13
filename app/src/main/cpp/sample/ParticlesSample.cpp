//
// Created by dev on 2023/6/13.
//

#include <gtc/matrix_transform.hpp>
#include "ParticlesSample.h"
#include "GLUtils.h"
#include "LogUtils.h"

ParticlesSample::ParticlesSample() {
    m_SamplerLoc = GL_NONE;
    m_MVPMatLoc  = GL_NONE;

    m_TextureId = GL_NONE;
    m_VaoId     = GL_NONE;

    m_AngleX = 0;
    m_AngleY = 0;

    m_ScaleX = 1.0f;
    m_ScaleY = 1.0f;

    m_LastUsedParticle    = 0;
    m_pParticlesPosData   = nullptr;
    m_pParticlesColorData = nullptr;

}

ParticlesSample::~ParticlesSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
    if (m_pParticlesColorData != nullptr) {
        delete[] m_pParticlesColorData;
    }
    if (m_pParticlesColorData != nullptr) {
        delete[] m_pParticlesColorData;
    }


}

void ParticlesSample::LoadImage(NativeImage *pImage) {
    LOGCATI("ParticlesSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage != nullptr) {
        m_RenderImage.width  = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::copyNativeImage(pImage, &m_RenderImage);
    }
}

void ParticlesSample::Init() {
    if (m_ProgramObj){
        return;
    }

    //  create RGBA texture.
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    // unbind
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    char vShaderStr[] =
                 "#version 300 es\n"
                 "precision mediump float;\n"
                 "layout(location = 0) in vec3 a_vertex;\n"
                 "layout(location = 1) in vec2 a_texCoord;\n"
                 "layout(location = 2) in vec3 a_offset;\n"
                 "layout(location = 3) in vec4 a_particlesColor;\n"
                 "uniform mat4 u_MVPMatrix;\n"
                 "out vec2 v_texCoord;\n"
                 "out vec4 v_color;\n"
                 "void main()\n"
                 "{\n"
                 "// vec3(0.0, 0.95, 0.0) 是为了使粒子整体向 y 轴负方向有一个偏移 \n"
                 "    gl_Position = u_MVPMatrix * vec4(a_vertex - vec3(0.0, 0.95, 0.0) + a_offset, 1.0);\n"
                 "    v_texCoord = a_texCoord;\n"
                 "    v_color = a_particlesColor;\n"
                 "}";

    char fShaderStr[] =
                 "#version 300 es\n"
                 "precision mediump float;\n"
                 "in vec2 v_texCoord;\n"
                 "in vec4 v_color;\n"
                 "layout(location = 0) out vec4 outColor;\n"
                 "uniform sampler2D s_TextureMap;\n"
                 "void main()\n"
                 "{\n"
                 "    outColor = texture(s_TextureMap, v_texCoord) * v_color;\n"
                 "}";

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    if (m_ProgramObj) {
        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
        m_MVPMatLoc  = glGetUniformLocation(m_ProgramObj, "u_MVPMatrix");
    } else {
        LOGCATE("ParticlesSample::Init create program fail");
        return;
    }

    // 一个粒子有 x,y,z ,三个位置点
    m_pParticlesPosData   = new GLfloat[MAX_PARTICLES * 3];
    // 一个粒子有 rgba 四个颜色通道
    m_pParticlesColorData = new GLubyte[MAX_PARTICLES * 4];

    // 创建500个粒子
    for (auto &particle: m_ParticlesContainer) {
        GenerateNewParticle(particle);
    }

    /**
     * 顶点数据
     */
    static const GLfloat g_vertex_buffer_data[] = {
            //position                               //texture coord
            -0.05f, -0.05f, -0.05f, 0.0f, 0.0f,
            0.05f, -0.05f, -0.05f, 1.0f, 0.0f,
            0.05f, 0.05f, -0.05f, 1.0f, 1.0f,
            0.05f, 0.05f, -0.05f, 1.0f, 1.0f,
            -0.05f, 0.05f, -0.05f, 0.0f, 1.0f,
            -0.05f, -0.05f, -0.05f, 0.0f, 0.0f,
            -0.05f, -0.05f, 0.05f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.05f, 1.0f, 0.0f,
            0.05f, 0.05f, 0.05f, 1.0f, 1.0f,
            0.05f, 0.05f, 0.05f, 1.0f, 1.0f,
            -0.05f, 0.05f, 0.05f, 0.0f, 1.0f,
            -0.05f, -0.05f, 0.05f, 0.0f, 0.0f,
            -0.05f, 0.05f, 0.05f, 1.0f, 0.0f,
            -0.05f, 0.05f, -0.05f, 1.0f, 1.0f,
            -0.05f, -0.05f, -0.05f, 0.0f, 1.0f,
            -0.05f, -0.05f, -0.05f, 0.0f, 1.0f,
            -0.05f, -0.05f, 0.05f, 0.0f, 0.0f,
            -0.05f, 0.05f, 0.05f, 1.0f, 0.0f,
            0.05f, 0.05f, 0.05f, 1.0f, 0.0f,
            0.05f, 0.05f, -0.05f, 1.0f, 1.0f,
            0.05f, -0.05f, -0.05f, 0.0f, 1.0f,
            0.05f, -0.05f, -0.05f, 0.0f, 1.0f,
            0.05f, -0.05f, 0.05f, 0.0f, 0.0f,
            0.05f, 0.05f, 0.05f, 1.0f, 0.0f,
            -0.05f, -0.05f, -0.05f, 0.0f, 1.0f,
            0.05f, -0.05f, -0.05f, 1.0f, 1.0f,
            0.05f, -0.05f, 0.05f, 1.0f, 0.0f,
            0.05f, -0.05f, 0.05f, 1.0f, 0.0f,
            -0.05f, -0.05f, 0.05f, 0.0f, 0.0f,
            -0.05f, -0.05f, -0.05f, 0.0f, 1.0f,
            -0.05f, 0.05f, -0.05f, 0.0f, 1.0f,
            0.05f, 0.05f, -0.05f, 1.0f, 1.0f,
            0.05f, 0.05f, 0.05f, 1.0f, 0.0f,
            0.05f, 0.05f, 0.05f, 1.0f, 0.0f,
            -0.05f, 0.05f, 0.05f, 0.0f, 0.0f,
            -0.05f, 0.05f, -0.05f, 0.0f, 1.0f,
    };

    // 粒子顶点
    glGenBuffers(1, &m_ParticlesVertexVboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVertexVboId);
    // 因为每次实例化渲染粒子时，都要更新 a_offset和 a_particlesColor实例化数组，所以设置其对应的 VBO 为动态类型 GL_DYNAMIC_DRAW 。
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data,
                 GL_STATIC_DRAW);
    GO_CHECK_GL_ERROR()

    // 粒子位置
    glGenBuffers(1, &m_ParticlesPosVboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesPosVboId);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 3 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    GO_CHECK_GL_ERROR()

    // 粒子颜色
    glGenBuffers(1, &m_ParticlesColorVboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesColorVboId);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), nullptr, GL_DYNAMIC_DRAW);
    GO_CHECK_GL_ERROR()

    // vao
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);
    GO_CHECK_GL_ERROR()

    // layout(location = 0) in vec3 a_vertex
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVertexVboId);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(GLfloat),
            (void *) 0
                         );
    GO_CHECK_GL_ERROR();

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVertexVboId);
    glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(GLfloat),
            (const void *) (3 * sizeof(GLfloat))
                         );
    GO_CHECK_GL_ERROR();

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesPosVboId);
    glVertexAttribPointer(
            2,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *) 0
                         );
    GO_CHECK_GL_ERROR();

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesColorVboId);
    glVertexAttribPointer(
            3,
            4,
            GL_UNSIGNED_BYTE,
            GL_TRUE,
            0,
            (void *) 0
                         );
    GO_CHECK_GL_ERROR();

    // 表示实例化绘制时对 index =0 的属性不更新
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);
    // 用于指定 index = 2 的属性为实例化数组，1 表示每绘制一个实例，更新一次数组中的元素。
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    GO_CHECK_GL_ERROR()
    glBindVertexArray(GL_NONE);
    GO_CHECK_GL_ERROR()

}

void ParticlesSample::Draw(int screenW, int screenH) {
    if (m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) {
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glDisable(GL_BLEND);
    UpdateMVPMatrix(m_MVPMatrix, m_AngleX, m_AngleY, (float) screenW / screenH);

    int particleCount = UpdateParticles();
    glUseProgram(m_ProgramObj);
    glBindVertexArray(m_VaoId);
    //  todo 为什么不这样写： &m_MVPMatrix
    glUniformMatrix4fv(m_MVPMatLoc, 1, GL_FALSE, &m_MVPMatrix[0][0]);

    // bind rgba map.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glUniform1i(m_SamplerLoc, 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, particleCount);


}

void ParticlesSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        glDeleteVertexArrays(1, &m_VaoId);
        glDeleteTextures(1, &m_TextureId);
        glDeleteBuffers(1, &m_ParticlesPosVboId);
        glDeleteBuffers(1, &m_ParticlesVertexVboId);
        glDeleteBuffers(1, &m_ParticlesPosVboId);
        m_ProgramObj = GL_NONE;
    }

}

void
ParticlesSample::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {
    GLSampleBase::UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    m_AngleX = static_cast<int >(rotateX);
    m_AngleY = static_cast<int >(rotateY);
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;

}

void ParticlesSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio) {

    angleX %= 360;
    angleY %= 360;
    // 转化为弧度
    auto radiansX = static_cast<float>(MATH_PI / 180.0f * angleX);
    auto radiansY = static_cast<float>(MATH_PI / 180.0f * angleY);

    // Projection matrix:
    glm::mat4 Projection = glm::perspective(45.0f, ratio, 0.1f, 100.f);
    // View matrix
    glm::mat4 View       = glm::lookAt(
            glm::vec3(0, 6, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 0, 1));

    // Model matrix
    glm::mat4 Model = glm::mat4(1.0f);
    Model     = glm::scale(Model, glm::vec3(m_ScaleX, m_ScaleX, m_ScaleX));
    // 旋转
    Model     = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model     = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    // 平移
    Model     = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
    mvpMatrix = Projection * View * Model;

}

int ParticlesSample::FindUnusedParticle() {
    for (int i = m_LastUsedParticle; i < MAX_PARTICLES; ++i) {
        if (m_ParticlesContainer[i].life <= 0) {
            m_LastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i < m_LastUsedParticle; i++) {
        if (m_ParticlesContainer[i].life <= 0) {
            m_LastUsedParticle = i;
            return i;
        }
    }

    return -1;
}

void ParticlesSample::SortParticles() {
    std::sort(&m_ParticlesContainer[0], &m_ParticlesContainer[MAX_PARTICLES]);
}


/**
 *  更新粒子(更新粒子的位置、运动速度和生命值)，然后更新实例化数组：
 * @return
 */
int ParticlesSample::UpdateParticles() {
    LOGCATI("ParticlesSample::UpdateParticles");
    for (int i              = 0; i < 300; ++i) {
        int particleIndex = FindUnusedParticle();
        if (particleIndex >= 0) {
            GenerateNewParticle(m_ParticlesContainer[particleIndex]);
        }
    }
    // Simulate all particles
    int      particlesCount = 0;
    for (int i              = 0; i < MAX_PARTICLES; ++i) {
        Particle &p = m_ParticlesContainer[i];
        if (p.life <= 0.0f) {
            continue;
        }
        // 得儿塔
        float     delta = 0.1f;
        // decrease
        glm::vec3 speed = glm::vec3(p.dxSpeed, p.dySpeed, p.dzSpeed);
        glm::vec3 pos   = glm::vec3(p.dx, p.dy, p.dz);
        // 生命周期，自减.
        p.life -= delta;

        if (p.life > 0.0f) {
            // 模拟简单的物理：重力，不包含碰撞
            speed += glm::vec3(0.0f, 0.081f, 0.0f) * delta * 0.5f;
            pos += speed * delta;
            p.dxSpeed = speed.x;
            p.dySpeed = speed.y;
            p.dzSpeed = speed.z;

            // 更新粒子位置
            p.dx = pos.x;
            p.dy = pos.y;
            p.dz = pos.z;

            // 好奇，这是什么规律
            m_pParticlesPosData[3 * particlesCount + 0] = p.dx;
            m_pParticlesPosData[3 * particlesCount + 1] = p.dy;
            m_pParticlesPosData[3 * particlesCount + 2] = p.dz;

            // 不更新粒子的颜色
            m_pParticlesColorData[4 * particlesCount + 0] = p.r;
            m_pParticlesColorData[4 * particlesCount + 1] = p.g;
            m_pParticlesColorData[4 * particlesCount + 2] = p.b;
            m_pParticlesColorData[4 * particlesCount + 3] = p.a;
        }
        particlesCount++;
    }


    glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesPosVboId);
    // buffer orphaning ,a common way to improve streaming perf, see above link for details.
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    GO_CHECK_GL_ERROR()
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(GLfloat) * 3, m_pParticlesPosData);
    GO_CHECK_GL_ERROR()

    glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesColorVboId);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(GLubyte) * 4,
                    m_pParticlesColorData);
    GO_CHECK_GL_ERROR()


    return particlesCount;
}

void ParticlesSample::GenerateNewParticle(Particle &particle) {
    particle.life           = 5.0f;
    particle.cameraDistance = -1.0f;

    particle.dx = (rand() % 2000 - 1000.0f) / 3000.0f;
    particle.dy = (rand() % 2000 - 1000.0f) / 3000.0f;
    particle.dz = (rand() % 2000 - 1000.0f) / 3000.0f;

    float     spread    = 1.5f;
    glm::vec3 maindir   = glm::vec3(0.0, 2.0f, 0.0f);
    glm::vec3 randomdir = glm::vec3(
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f);

    // tmd 向量相加
    glm::vec3 speed = maindir + randomdir * spread;
    particle.dxSpeed = speed.x;
    particle.dySpeed = speed.y;
    particle.dzSpeed = speed.z;

    // color
    particle.r = static_cast<unsigned char>(rand() % 256);
    particle.g = static_cast<unsigned char>(rand() % 256);
    particle.b = static_cast<unsigned char>(rand() % 256);
    particle.a = static_cast<unsigned char>((rand() % 256) / 3);
}
