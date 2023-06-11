//
// Created by dev on 2023/6/10.
//

#ifndef STUDYOPENGL3_0_INSTANCINGSAMPLE_H
#define STUDYOPENGL3_0_INSTANCINGSAMPLE_H

#include <detail/type_mat.hpp>
#include "GLSampleBase.h"
#include "ImageDef.h"


class InstancingSample :public GLSampleBase{
public:
    InstancingSample();

    virtual ~InstancingSample();

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

private:
    GLuint m_InstanceVbo;
    GLuint m_VerticesVbo;
    GLuint m_Vao;

};


#endif //STUDYOPENGL3_0_INSTANCINGSAMPLE_H
