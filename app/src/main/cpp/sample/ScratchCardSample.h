//
// Created by dev on 2023/6/18.
//

#ifndef STUDYOPENGL3_0_SCRATCHCARDSAMPLE_H
#define STUDYOPENGL3_0_SCRATCHCARDSAMPLE_H


#include <detail/type_mat4x4.hpp>
#include <detail/type_mat.hpp>
#include <vector>
#include "GLSampleBase.h"

#define TRIANGLE_NUM 43
#define EFFECT_RADIUS 0.03

class ScratchCardSample : public GLSampleBase {
public:
    ScratchCardSample();

    virtual ~ScratchCardSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    virtual void SetTouchLocation(float x, float y);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    void CalculateMesh(glm::vec2 pre, glm::vec2 cur);

private:
    GLuint      m_TextureId;
    GLint       m_SamplerLoc;
    GLint       m_MVPMatLoc;
    GLuint      m_VaoId;
    GLuint      m_VboIds[2];
    NativeImage m_RenderImage;
    glm::mat4   m_MVPMatrix;

    int m_AngleX;
    int m_AngleY;

    float     m_ScaleX;
    float     m_ScaleY;



    std::vector<glm::vec4> m_PointVector;


    bool      m_bReset;
};


#endif //STUDYOPENGL3_0_SCRATCHCARDSAMPLE_H
