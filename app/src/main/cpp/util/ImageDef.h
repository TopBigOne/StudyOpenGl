//
// Created by dev on 2023/5/25.
//

#ifndef STUDYOPENGL3_0_IMAGEDEF_H
#define STUDYOPENGL3_0_IMAGEDEF_H

#include <malloc.h>
#include <string>
#include <unistd.h>
#include <stdio.h>

#define IMAGE_FORMAT_RGBA           0x01
#define IMAGE_FORMAT_NV21           0x02
#define IMAGE_FORMAT_NV12           0x03
#define IMAGE_FORMAT_I420           0x04
#define IMAGE_FORMAT_YUYV           0x05
#define IMAGE_FORMAT_GRAY           0x06
#define IMAGE_FORMAT_I444           0x07
#define IMAGE_FORMAT_P010           0x08

#define IMAGE_FORMAT_RGBA_EXT       "RGB32"
#define IMAGE_FORMAT_NV21_EXT       "NV21"
#define IMAGE_FORMAT_NV12_EXT       "NV12"
#define IMAGE_FORMAT_I420_EXT       "I420"
#define IMAGE_FORMAT_YUYV_EXT       "YUYV"
#define IMAGE_FORMAT_GRAY_EXT       "GRAY"
#define IMAGE_FORMAT_I444_EXT       "I444"
#define IMAGE_FORMAT_P010_EXT       "P010" //16bit NV21


typedef struct NativeRectF {
    float left;
    float top;
    float right;
    float bottom;

    NativeRectF() {
        left = top = right = bottom = 0.0f;
    }

} RectF;


struct SizeF {
    float width;
    float height;

    SizeF() {
        width = height = 0;
    }
};

struct NativeImage {
    int width;
    int height;
    int format;
    uint8_t *ppPlane[3];

    NativeImage() {
        width = 0;
        height = 0;
        format = 0;

        // y
        ppPlane[0] = nullptr;
        // u
        ppPlane[1] = nullptr;
        // v
        ppPlane[2] = nullptr;
    }
};

class NativeImageUtil {
public:
    static void AllocNativeImage(NativeImage *pImage) {

    }

    static void FreeNativeImage(NativeImage *pImage) {
        if (pImage == nullptr || pImage->ppPlane[0] == nullptr) {
            return;
        }
        free(pImage->ppPlane[0]);
        pImage->ppPlane[0] = nullptr;
        pImage->ppPlane[1] = nullptr;
        pImage->ppPlane[2] = nullptr;

    }

    static void copyNativeImage(NativeImage *SrcImg, NativeImage *pDstImg) {

    }

    static void DumpNativeImage(NativeImage *pImage, const char *pPath, const char *pFileName) {

    }

    static void LoadNativeImage(NativeImage *pSrcImg, const char *pPath) {

    }

    static void ConvertP010ToNV21(NativeImage *pP010Img,NativeImage*pNV21Img){

    }

    static void ConvertP010PlaneTo8Bit(uint16_t *pP010Img,NativeImage pNV21Img){

    }
    static void ConvertNV21ToP010(NativeImage *pNV21Img,NativeImage pP010Img){

    }

};


#endif //STUDYOPENGL3_0_IMAGEDEF_H
