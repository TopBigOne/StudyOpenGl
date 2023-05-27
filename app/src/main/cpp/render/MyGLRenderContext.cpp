//
// Created by dev on 2023/5/25.
//

#include "MyGLRenderContext.h"
#include "LogUtils.h"

MyGLRenderContext *MyGLRenderContext::m_pContext = nullptr;

MyGLRenderContext::MyGLRenderContext() {
    m_pCurSample = new TriangleSample;
    m_pBeforeSample = nullptr;


}


void MyGLRenderContext::SetImageData(int format, int width, int height, uint8_t *pData) {

}

void MyGLRenderContext::SetImageDataWithIndex(int index, int format, int width, int height,
                                              uint8_t *pData) {

}

void MyGLRenderContext::SetParamsInt(int paramType, int value0, int value1) {
    LOGCATE("MyGLRenderContext::SetParamsInt paramType = %d, value0 = %d, value1 = %d", paramType,
            value0, value1);
    if (paramType == SAMPLE_TYPE) {
        switch (value0) {
            // 画个三角形
            case SAMPLE_TYPE_KEY_TRIANGLE:
                m_pCurSample = new TriangleSample();
                break;


        }
    }


}

void MyGLRenderContext::SetParamsFloat(int paramType, float value0, float value1) {

}

void MyGLRenderContext::SetParamsShortArr(short *const pShortArr, int arrSize) {

}

void
MyGLRenderContext::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {

}

void MyGLRenderContext::OnSurfaceCreated() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}


void MyGLRenderContext::OnSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
}

void MyGLRenderContext::OnDrawFrame() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    if (m_pBeforeSample) {
        m_pBeforeSample->Destroy();
        delete m_pBeforeSample;
        m_pBeforeSample = nullptr;
    }
    if (m_pCurSample) {
        m_pCurSample->Init();
        m_pCurSample->Draw(m_ScreenW, m_ScreenH);
    }

}

MyGLRenderContext *MyGLRenderContext::GetInstance() {
    LOGCATE("MyGLRenderContext::GetInstance");
    if (m_pContext == nullptr) {
        m_pContext = new MyGLRenderContext();
    }
    return m_pContext;
}

void MyGLRenderContext::DestroyInstance() {
    LOGCATE("MyGLRenderContext::DestroyInstance");
    if (m_pContext) {
        delete m_pContext;
        m_pContext = nullptr;
    }
}


MyGLRenderContext::~MyGLRenderContext() {
    if (m_pCurSample) {
        delete m_pCurSample;
        m_pCurSample = nullptr;
    }
    if (m_pBeforeSample) {
        delete m_pBeforeSample;
        m_pBeforeSample = nullptr;
    }


}
