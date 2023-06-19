//
// Created by dev on 2023/6/18.
//

#ifndef STUDYOPENGL3_0_VISUALIZEAUDIOSAMPLE_H
#define STUDYOPENGL3_0_VISUALIZEAUDIOSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <mutex>
#include <vec3.hpp>
#include <vec2.hpp>
#include "GLSampleBase.h"

using namespace glm;

#define MAX_AUDIO_LEVEL 2000 // 音频最大级别
#define RESAMPLE_LEVEL  112  // 从采样级别

/**
 * 音频可视化。
 */
class VisualizeAudioSample : public GLSampleBase{

public:
    VisualizeAudioSample();

    virtual ~VisualizeAudioSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void LoadShortArrData(short *const pShortArr, int arrSize);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    void UpdateMesh();

private:
    GLint     m_SamplerLoc;
    GLint     m_MVPMatLoc;
    GLuint    m_VaoId;
    GLuint    m_VboIds[2];
    glm::mat4 m_MVPMatrix;

    int   m_AngleX;
    int   m_AngleY;
    float m_ScaleX;
    float m_ScaleY;
    short *m_pCurAudioData;
    short *m_pAudioBuffer;
    int   m_AudioDataSize;

    std::mutex              m_Mutex;
    std::condition_variable m_Cond;

    glm::vec3 *m_pVerticesCoords;
    glm::vec2 *m_pTextureCoords;

    int m_RenderDataSize;

    int           m_FrameIndex;
    volatile bool m_bAudioDataReady;


};


#endif //STUDYOPENGL3_0_VISUALIZEAUDIOSAMPLE_H
