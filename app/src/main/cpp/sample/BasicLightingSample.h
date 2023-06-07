//
// Created by dev on 2023/6/5.
//

#ifndef STUDYOPENGL3_0_BASICLIGHTINGSAMPLE_H
#define STUDYOPENGL3_0_BASICLIGHTINGSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "GLSampleBase.h"

/**
 * 光照 基础
 */
class BasicLightingSample : public GLSampleBase {
public:
    BasicLightingSample();

    virtual ~BasicLightingSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:

    // 纹理ID
    GLuint m_TextureId;
    GLint  m_SamplerLoc;
    GLint  m_MVPMatLoc;
    GLint  m_ModelMatrixLoc;
    GLint  m_LightPosLoc;
    GLint  m_LightColorLoc;
    GLint  m_ViewPosLoc;

    GLuint      m_VaoId;
    GLuint      m_VboIds[1];
    NativeImage m_RenderImage;
    glm::mat4   m_MVPMatrix;
    glm::mat4   m_ModelMatrix;

    int   m_AngleX;
    int   m_AngleY;
    float m_ScaleX;
    float m_ScaleY;


};


#endif //STUDYOPENGL3_0_BASICLIGHTINGSAMPLE_H
