//
// Created by dev on 2023/6/16.
//

#ifndef STUDYOPENGL3_0_SKYBOXSAMPLE_H
#define STUDYOPENGL3_0_SKYBOXSAMPLE_H

#include "GLSampleBase.h"
#include <detail/type_mat4x4.hpp>
#include <detail/type_mat.hpp>

class SkyBoxSample : public GLSampleBase {

public:
    SkyBoxSample();

    virtual ~SkyBoxSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void LoadMultiImageWithIndex(int index, NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotatex, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float scale, float ratio);

private:
    GLuint m_TextureId;
    GLuint m_CubeProgramObj;

    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;
    GLint m_CubeSamplerLoc;
    GLint m_CubeMVPMatLoc;
    GLint m_CubeModelMatLoc;
    GLint m_ViewPosLoc;

    GLuint m_CubeVaoId;
    GLuint m_CubeVboId;
    GLuint m_SkyBoxVaoId;
    GLuint m_SkyBoxVboId;

    NativeImage m_RenderImage;
    /**
     * 图片数组，大小为6；
     */
    NativeImage m_pSkyBoxRenderImg[6];

    glm::mat4 m_MVPMatrix;
    glm::mat4 m_ModelMatrix;

    int m_AngleX;
    int m_AngleY;

    float m_ScaleX;
    float m_ScaleY;


};


#endif //STUDYOPENGL3_0_SKYBOXSAMPLE_H
