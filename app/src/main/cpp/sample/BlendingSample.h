//
// Created by dev on 2023/6/9.
//

#ifndef STUDYOPENGL3_0_BLENDINGSAMPLE_H
#define STUDYOPENGL3_0_BLENDINGSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <GLSampleBase.h>
#include <vector>
#include <map>

#define RENDER_IMG_NUM 3

class BlendingSample : public GLSampleBase {
public:
    BlendingSample();

    virtual ~BlendingSample();

    virtual void LoadMultiImageWithIndex(int index, NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    virtual void UpdateMatrix(glm::mat4 &mvpMartix, int angleXRotate, int angleYRotate, float scale,
                              glm::vec3 transVec3, float ratio);

private:
    GLuint m_TextureIds[RENDER_IMG_NUM];
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;

    GLuint      m_VaoIds[3];
    GLuint      m_VboIds[3];
    NativeImage m_RenderImages[RENDER_IMG_NUM];
    glm::mat4   m_MVPMatrix;

    std::vector<glm::vec3>       windowsTrans;
    std::map<GLfloat, glm::vec3> sorted;

    int m_AngleX;
    int m_AngleY;


};


#endif //STUDYOPENGL3_0_BLENDINGSAMPLE_H
