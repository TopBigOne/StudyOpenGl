//
// Created by dev on 2023/5/29.
//

#ifndef STUDYOPENGL3_0_NV21TEXTUREMAPSAMPLE_H
#define STUDYOPENGL3_0_NV21TEXTUREMAPSAMPLE_H

#include "GLSampleBase.h"
#include "../util/ImageDef.h"

class NV21TextureMapSample : public GLSampleBase {
public:
    NV21TextureMapSample() {
        m_yTextureId = 0;
        m_uvTextureId = 0;

        m_ySampleLocation = 0;
        m_uvSampleLocation = 0;
    }

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    ~NV21TextureMapSample() {
        NativeImageUtil::FreeNativeImage(&m_RenderImage);
    }

private:
    GLuint m_yTextureId;
    GLuint m_uvTextureId;

    GLint m_ySampleLocation;
    GLint m_uvSampleLocation;

    NativeImage m_RenderImage;


};


#endif //STUDYOPENGL3_0_NV21TEXTUREMAPSAMPLE_H
