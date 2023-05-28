//
// Created by dev on 2023/5/28.
//

#ifndef STUDYOPENGL3_0_TEXTUREMAPSAMPLE_H
#define STUDYOPENGL3_0_TEXTUREMAPSAMPLE_H

#include "GLSampleBase.h"
#include "../util/ImageDef.h"

class TextureMapSample : public GLSampleBase {
public:
    TextureMapSample();

    virtual ~TextureMapSample();

    void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

private:
    GLuint m_TextureId;
    GLint m_SampleLoc;
    NativeImage m_RenderImage;

};


#endif //STUDYOPENGL3_0_TEXTUREMAPSAMPLE_H
