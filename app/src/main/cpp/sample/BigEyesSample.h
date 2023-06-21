//
// Created by dev on 2023/6/20.
//

#ifndef STUDYOPENGL3_0_BIGEYESSAMPLE_H
#define STUDYOPENGL3_0_BIGEYESSAMPLE_H


#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <MySyncLock.h>
#include "GLSampleBase.h"

class BigEyesSample : public GLSampleBase {
public:
    BigEyesSample();
    virtual ~BigEyesSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);
    virtual void UpdateEyeOffset(float offset);

private:
    GLuint      m_TextureId;
    GLint       m_SamplerLoc;
    GLint       m_MVPMatLoc;
    GLuint      m_VaoId;
    GLuint      m_VboIds[3];
    NativeImage m_RenderImage;
    glm::mat4   m_MVPMatrix;

    int   m_AngleX;
    int   m_AngleY;
    float m_ScaleX;
    float m_ScaleY;

    float m_EyeOffset;

    /**
     * frame index 是干嘛的？
     */
    int m_FrameIndex;
};


#endif //STUDYOPENGL3_0_BIGEYESSAMPLE_H
