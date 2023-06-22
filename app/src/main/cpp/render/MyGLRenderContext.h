//
// Created by dev on 2023/5/25.
//


#ifndef STUDYOPENGL3_0_MYGLRENDERCONTEXT_H
#define STUDYOPENGL3_0_MYGLRENDERCONTEXT_H

#include <stdint.h>
#include <GLES3/gl3.h>
// 三角形
#include <../sample/TriangleSample.h>

#include <TextureMapSample.h>
#include <NV21TextureMapSample.h>
#include <VaoSample.h>
#include <FBOSample.h>
#include <FBOBlitSample.h>

#include <TransformFeedbackSample.h>
#include <FBOLegLengthenSample.h>
#include <CoordSystemSample.h>
#include <BasicLightingSample.h>
#include <DepthTestingSample.h>
#include <BlendingSample.h>
#include <InstancingSample.h>
#include <Instancing3DSample.h>
#include <ParticlesSample.h>
#include <PBOSample.h>
#include <MRTSample.h>
#include <SkyBoxSample.h>
#include <StencilTestingSample.h>
#include "StencilTestingSample.h"
#include "AvatarSample.h"
#include "ScratchCardSample.h"
#include "VisualizeAudioSample.h"
#include "RotaryHeadSample.h"
#include "BigEyesSample.h"
#include <ShockWaveSample.h>
#include <FaceSlenderSample.h>
#include <BezierCurveSample.h>

class MyGLRenderContext { // implicit instantiation of undefined template
public:
    MyGLRenderContext();

    ~MyGLRenderContext();

public:
    void SetImageData(int format, int width, int height, uint8_t *pData);

    void SetImageDataWithIndex(int index, int format, int width, int height, uint8_t *pData);

    void SetParamsInt(int paramType, int value0, int value1);

    void SetParamsFloat(int paramType, float value0, float value1);

    void UpdateEyeOffset(float offset);

    void SetParamsShortArr(short *const pShortArr, int arrSize);

    void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    static MyGLRenderContext *GetInstance();

    static void DestroyInstance();


private:
    static MyGLRenderContext *m_pContext;
    GLSampleBase             *m_pBeforeSample;
    GLSampleBase             *m_pCurSample;
    int                      m_ScreenW;
    int                      m_ScreenH;
};


#endif //STUDYOPENGL3_0_MYGLRENDERCONTEXT_H
