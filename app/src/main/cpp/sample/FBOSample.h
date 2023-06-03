//
// Created by dev on 2023/6/1.
//

#ifndef STUDYOPENGL3_0_FBOSAMPLE_H
#define STUDYOPENGL3_0_FBOSAMPLE_H

#include "GLSampleBase.h"
#include "../util/ImageDef.h"

#define VERTEX_POS_INDEX 0
#define TEXTURE_POS_INDEX 1


class FBOSample : public GLSampleBase {
public:
    FBOSample();

    virtual ~FBOSample();

    virtual void LoadImage(NativeImage *p_Image);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual bool CreateFrameBufferObj();
    virtual void initVBO(const GLfloat *vVertices, const GLfloat *vTexCoors, const GLfloat *vFboTexCoors,
                 const GLushort *indices) ;

private:
    GLuint      m_ImageTextureId;
    GLuint      m_FboTextureId;
    GLuint      m_FboId;
    GLuint      m_VaoIds[2];

    GLuint      m_VboIds[4];
    GLuint      m_VeoId;

    GLint       m_SampleLoc;
    NativeImage m_RenderImage;
    GLuint      m_FboProgramObj;
    GLuint      m_FboVertexShader;
    GLuint      m_FboFragmentShader;
    GLint       m_FboSamplerLoc;


};


#endif //STUDYOPENGL3_0_FBOSAMPLE_H
