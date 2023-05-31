//
// Created by dev on 2023/5/30.
//

#ifndef STUDYOPENGL3_0_VAOSAMPLE_H
#define STUDYOPENGL3_0_VAOSAMPLE_H

#include "GLSampleBase.h"

class VaoSample : public GLSampleBase {
public:
    VaoSample();

    ~VaoSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

private:
    GLuint m_VaoId;
    GLuint m_VboIds[2];
};


#endif //STUDYOPENGL3_0_VAOSAMPLE_H
