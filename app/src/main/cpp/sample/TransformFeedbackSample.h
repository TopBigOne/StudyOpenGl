//
// Created by dev on 2023/6/5.
//

#ifndef STUDYOPENGL3_0_TRANSFORMFEEDBACKSAMPLE_H
#define STUDYOPENGL3_0_TRANSFORMFEEDBACKSAMPLE_H

#include "GLSampleBase.h"
#include "../util/ImageDef.h"

class TransformFeedbackSample : public GLSampleBase {
public:
    TransformFeedbackSample();

    virtual ~TransformFeedbackSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

private:
    GLuint m_ImageTextureId;
    GLuint m_VaoId;
    GLuint m_VboIds[2] = {GL_NONE};;
    GLint       m_SamplerLoc;
    GLuint      m_TransFeedbackObjId;
    GLuint      m_TransFeedbackBufId;
    NativeImage m_RenderImage;
};


#endif //STUDYOPENGL3_0_TRANSFORMFEEDBACKSAMPLE_H
