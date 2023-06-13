//
// Created by dev on 2023/6/13.
//

#ifndef STUDYOPENGL3_0_PARTICLESSAMPLE_H
#define STUDYOPENGL3_0_PARTICLESSAMPLE_H


#include <detail/type_mat4x4.hpp>
#include <detail/type_mat.hpp>
#include "GLSampleBase.h"

// 最多500个粒子
#define MAX_PARTICLES 500

struct Particle {
    // offset
    GLfloat dx, dy, dz;
    // speed
    GLfloat dxSpeed;
    GLfloat dySpeed;
    GLfloat dzSpeed;
    // 生命值，也就是这个粒子能存活多久
    GLfloat life;
    // color
    GLubyte r, g, b, a;
    GLfloat cameraDistance;

    // init
    Particle() {
        dx = 0.0f;
        dy = 0.0f;
        dz = 0.0f;

        r = static_cast<GLubyte>(1.0f);
        g = static_cast<GLubyte>(1.0f);
        b = static_cast<GLubyte>(1.0f);
        a = static_cast<GLubyte>(1.0f);

        dxSpeed = 1.0f;
        dySpeed = 1.0f;
        dzSpeed = 1.0f;

        life = 5.0f;
    }


    /**
     * operator : 我不会。
     * @param that
     * @return
     */
    bool operator<(const Particle &that) const {
        // Sort in reverse order : far particles drawn first.
        return this->cameraDistance > that.cameraDistance;
    }


};

/**
 * 粒子特效
 */
class ParticlesSample : public GLSampleBase {

public:

    ParticlesSample();

    virtual ~ParticlesSample();
    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    virtual void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    int FindUnusedParticle();

    void SortParticles();

    /**
     * 更新粒子
     * @return
     */
    int UpdateParticles();

    void GenerateNewParticle(Particle &particle);

private:
    GLuint m_TextureId;
    // 着色器的2D 纹理采样
    GLint  m_SamplerLoc;
    // 着色器的 Model，View，Projection
    GLint  m_MVPMatLoc;

    GLuint      m_VaoId;
    //  粒子顶点 vbo
    GLuint      m_ParticlesVertexVboId;
    //  粒子位置 vbo
    GLuint      m_ParticlesPosVboId;
    //  粒子颜色 vbo
    GLuint      m_ParticlesColorVboId;
    NativeImage m_RenderImage;
    glm::mat4   m_MVPMatrix;

    // particles relation
    Particle m_ParticlesContainer[MAX_PARTICLES];
    GLfloat *m_pParticlesPosData;
    GLubyte *m_pParticlesColorData;
    int     m_LastUsedParticle;

    int m_AngleX;
    int m_AngleY;

    float m_ScaleX;
    float m_ScaleY;


};


#endif //STUDYOPENGL3_0_PARTICLESSAMPLE_H
