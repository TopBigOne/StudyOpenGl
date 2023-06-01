//
// Created by dev on 2023/5/31.
//

#ifndef STUDYOPENGL3_0_FBOBLITSAMPLE_H
#define STUDYOPENGL3_0_FBOBLITSAMPLE_H

#include "GLSampleBase.h"
#include "detail/type_mat.hpp"
#include "detail/type_mat4x4.hpp"

#define ATTACHMENT_NUMBER 4
#define VBO_NUM 3

class FBOBlitSample : public GLSampleBase {
public:
    FBOBlitSample();

    virtual ~FBOBlitSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    bool InitFBO();

    /**
     * 闪亮纹理
     */
    void BlitTexture();

private:
    GLuint      m_TextureId;
    GLuint      m_AttachTexIds[ATTACHMENT_NUMBER];
    GLuint      m_FBO;
    GLint      m_SamplerLoc;
    GLint      m_MVPMatLoc;
    GLuint      m_VaoId;
    GLuint      m_VboIds[VBO_NUM];
    NativeImage m_RenderImage;
    glm::mat4   m_MVPMatrix;

    int   m_AngleX;
    int   m_AngleY;
    float m_ScaleX;
    float m_ScaleY;


};


#endif //STUDYOPENGL3_0_FBOBLITSAMPLE_H
