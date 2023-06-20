//
// Created by dev on 2023/6/20.
//

#ifndef STUDYOPENGL3_0_ROTARYHEADSAMPLE_H
#define STUDYOPENGL3_0_ROTARYHEADSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "MySyncLock.h"
#include "CommonDef.h"
#include <atomic>
#include "GLSampleBase.h"
#include <vec3.hpp>
#include <vec2.hpp>

using namespace glm;
#define TRIANGLE_COUNT 32
#define KEY_POINTS_COUNT 9

class RotaryHeadSample : public GLSampleBase {

public:
    RotaryHeadSample();

    virtual ~RotaryHeadSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    /**
     *  计算网格
     * @param rotaryAngle 旋转角度
     */
    void CalculateMesh(float rotaryAngle);

    /**
     * 计算交点
     * @param inputPoint  头部边缘关键点
     * @param centerPoint  头部中心点
     * @return
     */
    vec2 CalculateIntersection(vec2 inputPoint, vec2 centerPoint);

    /**
     * 旋转关键点
     * @param input
     * @param rotaryAngle
     * @return
     */
    vec2 RotaryKeyPoint(vec2 input, float rotaryAngle);

private:
    GLuint      m_TextureId;
    GLint       m_SamplerLoc;
    GLint       m_MVPMatLoc;
    NativeImage m_RenderImage;
    mat4        m_MVPMatrix;

    int   m_AngleX;
    int   m_AngleY;
    float m_ScaleX;
    float m_ScaleY;

    int m_FrameIndex;

    vec3   m_Vertices[TRIANGLE_COUNT * 3]; //28个三角形
    vec2   m_TexCoords[TRIANGLE_COUNT * 3]; //28个三角形
    vec2   m_MeshPoints[24];
    vec2   m_KeyPointsInts[KEY_POINTS_COUNT]; //关键点对应的交点
    vec2   m_KeyPoints[KEY_POINTS_COUNT]; //关键点归一化
    GLuint m_VaoId;
    GLuint m_VboIds[2];


};


#endif //STUDYOPENGL3_0_ROTARYHEADSAMPLE_H
