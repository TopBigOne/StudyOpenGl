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

class MyGLRenderContext {
public:
    MyGLRenderContext();

    ~MyGLRenderContext();

public:
    void SetImageData(int format, int width, int height, uint8_t *pData);

    void SetImageDataWithIndex(int index, int format, int width, int height, uint8_t *pData);

    void SetParamsInt(int paramType, int value0, int value1);

    void SetParamsFloat(int paramType, float value0, float value1);

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
