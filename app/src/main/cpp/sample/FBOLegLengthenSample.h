//
// Created by dev on 2023/6/5.
//

#ifndef STUDYOPENGL3_0_FBOLEGLENGTHENSAMPLE_H
#define STUDYOPENGL3_0_FBOLEGLENGTHENSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "ImageDef.h"
#include "GLSampleBase.h"

typedef enum STRETCH_MODEL {

    VERTICAL_STRETCH_8_POINTS,
    VERTICAL_STRETCH_TOP_6_POINTS,
    VERTICAL_STRETCH_BOTTOM_6_POINTS,
    VERTICAL_STRETCH_4_POINTS,

    HORIZONTAL_STRETCH_8_POINTS,
    HORIZONTAL_STRETCH_LEFT_6_POINTS,
    HORIZONTAL_STRETCH_RIGHT_6_POINTS,
    HORIZONTAL_STRETCH_4_POINTS
} StretchMode;

class FBOLegLengthenSample : public GLSampleBase {
public:
    FBOLegLengthenSample();

    virtual ~FBOLegLengthenSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    bool CreateFrameBufferObj();


    void configStretchArea();

    void claculateStretchMode();

    void initMVP();

    void initShaderProgram();

    void initShaderData();

    /**
    * 处理拉伸
    * @param mode  拉伸类型
    */
    void handleStretch();

private:
    GLuint m_ImageTextureId;
    GLuint m_FboTextureId;
    GLuint m_FboId;
    GLuint m_VaoIds[2] = {GL_NONE};;
    GLuint m_VboIds[6] = {GL_NONE};;

    GLint       m_SamplerLoc;
    NativeImage m_RenderImage;

    // fbo.
    GLuint m_FboProgramObj;
    GLuint m_FboVertexShader;
    GLuint m_FboFragmentShader;
    GLint  m_FboSamplerLoc;

    glm::mat4 m_FboMVPMatrix;
    GLint     m_FboMVPMatLoc;

    glm::mat4 m_MVPMatrix;
    GLint     m_MVPMatLoc;

    GLfloat m_dt;
    bool    m_isgo;

    RectF       m_StretchRect;
    // 拉伸模式
    StretchMode m_StretchMode;

    // 是否为水平拉伸.
    bool m_bIsVerticalMode;


    void initDT();

    GLsizei handleIndexNum();
};


#endif //STUDYOPENGL3_0_FBOLEGLENGTHENSAMPLE_H
