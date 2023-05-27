//
// Created by dev on 2023/5/25.
//

#ifndef STUDYOPENGL3_0_TRIANGLESAMPLE_H
#define STUDYOPENGL3_0_TRIANGLESAMPLE_H

#include "GLSampleBase.h"
#include "../util/MySyncLock.h"
#include <ImageDef.h>

/**
 * 三角形
 */
class TriangleSample : public GLSampleBase {
public:
    TriangleSample();

    virtual ~TriangleSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();
};


#endif //STUDYOPENGL3_0_TRIANGLESAMPLE_H
