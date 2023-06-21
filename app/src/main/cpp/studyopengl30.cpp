#include <jni.h>

#include "util/LogUtils.h"
#include "render/MyGLRenderContext.h"
#include <EGLRender.h>

#define NATIVE_RENDER_CLASS_NAME "com/ya/opengl/studyopengl30/render/NativeRender"
#define NATIVE_BG_RENDER_CLASS_NAME "com/ya/opengl/studyopengl30/egl/NativeEglRender"


extern "C"
JNIEXPORT void JNICALL native_Init(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance();

}
/**
 * https://juejin.cn/post/7129798779196145672
 * reinterpret_cast:  用于进行没有任何关联之间的转换，比如一个字符指针转换为一个整形数
 */
extern "C"
JNIEXPORT void JNICALL native_SetImageData(JNIEnv *env, jobject thiz,
                                           jint format, jint width,
                                           jint height,
                                           jbyteArray imageData) {

    int len = env->GetArrayLength(imageData);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte *>(buf));
    MyGLRenderContext::GetInstance()->SetImageData(format, width, height, buf);


}
extern "C"
JNIEXPORT void JNICALL native_OnSurfaceCreated(JNIEnv *env,
                                               jobject thiz) {
    MyGLRenderContext::GetInstance()->OnSurfaceCreated();

}
extern "C"
JNIEXPORT void JNICALL native_OnSurfaceChanged(JNIEnv *env,
                                               jobject thiz,
                                               jint width,
                                               jint height) {
    MyGLRenderContext::GetInstance()->OnSurfaceChanged(width, height);

}
extern "C"
JNIEXPORT void JNICALL native_OnDrawFrame(JNIEnv *env,
                                          jobject thiz) {
    MyGLRenderContext::GetInstance()->OnDrawFrame();


}

extern "C"
JNIEXPORT void JNICALL native_UnInit(JNIEnv *env, jobject thiz) {

}

JNIEXPORT void JNICALL native_SetParamsInt
        (JNIEnv *env, jobject instance, jint paramType, jint value0, jint value1) {

    LOGCATE("native_SetParamsInt  paramType %d :",paramType);

    MyGLRenderContext::GetInstance()->SetParamsInt(paramType, value0, value1);
}



JNIEXPORT void JNICALL native_SetImageDataWithIndex
        (JNIEnv *env, jobject instance, jint index, jint format, jint width, jint height,
         jbyteArray imageData) {
    int len = env->GetArrayLength(imageData);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte *>(buf));
    MyGLRenderContext::GetInstance()->SetImageDataWithIndex(index, format, width, height, buf);
    delete[] buf;
    env->DeleteLocalRef(imageData);
}


JNIEXPORT void JNICALL native_SetParamsFloat
        (JNIEnv *env, jobject instance, jint paramType, jfloat value0, jfloat value1) {
    MyGLRenderContext::GetInstance()->SetParamsFloat(paramType, value0, value1);
}




JNIEXPORT void JNICALL native_SetAudioData
        (JNIEnv *env, jobject instance, jshortArray data) {
    int len = env->GetArrayLength(data);
    short *pShortBuf = new short[len];
    env->GetShortArrayRegion(data, 0, len, reinterpret_cast<jshort *>(pShortBuf));
    MyGLRenderContext::GetInstance()->SetParamsShortArr(pShortBuf, len);
    delete[] pShortBuf;
    env->DeleteLocalRef(data);
}



JNIEXPORT void JNICALL
native_UpdateTransformMatrix(JNIEnv *env, jobject instance, jfloat rotateX, jfloat rotateY,
                             jfloat scaleX, jfloat scaleY) {
    MyGLRenderContext::GetInstance()->UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
}

JNIEXPORT void JNICALL
native_UpdateEyeOffset(JNIEnv *env, jobject instance, jfloat eyeOffset) {
    MyGLRenderContext::GetInstance()->UpdateEyeOffset(eyeOffset);
}



JNIEXPORT void JNICALL native_EglRenderInit(JNIEnv *env, jobject instance) {
    EGLRender::GetInstance()->Init();

}


JNIEXPORT void JNICALL
native_EglRenderSetImageData(JNIEnv *env, jobject instance, jbyteArray data, jint width,
                             jint height) {
    int len = env->GetArrayLength(data);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(data, 0, len, reinterpret_cast<jbyte *>(buf));
    EGLRender::GetInstance()->SetImageData(buf, width, height);
    delete[] buf;
    // 删除局部引用
    env->DeleteLocalRef(data);
}


JNIEXPORT void JNICALL
native_EglRenderSetIntParams(JNIEnv *env, jobject instance, jint type, jint param) {
    EGLRender::GetInstance()->SetIntParams(type, param);

}

/*
 * Class:     com_byteflow_app_egl_NativeBgRender
 * Method:    native_EglRenderDraw
 * Signature: ()V
 */
JNIEXPORT void JNICALL native_EglRenderDraw(JNIEnv *env, jobject instance) {
    EGLRender::GetInstance()->Draw();
}

/*
 * Class:     com_byteflow_app_egl_NativeBgRender
 * Method:    natuve_BgRenderUnInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL natuve_BgRenderUnInit(JNIEnv *env, jobject instance) {
    EGLRender::GetInstance()->UnInit();
}



static JNINativeMethod g_RenderMethods[] = {
        {"native_Init",             "()V",      (void *) (native_Init)},
        {"native_UnInit",           "()V",      (void *) (native_UnInit)},
        {"native_SetImageData",     "(III[B)V", (void *) (native_SetImageData)},

        {"native_SetImageDataWithIndex", "(IIII[B)V", (void *) (native_SetImageDataWithIndex)},
        {"native_SetParamsInt",     "(III)V",   (void *) (native_SetParamsInt)},
        {"native_SetParamsFloat", "(IFF)V", (void *) (native_SetParamsFloat)},
        {"native_SetAudioData", "([S)V", (void *) (native_SetAudioData)},
        {"native_UpdateTransformMatrix", "(FFFF)V", (void *) (native_UpdateTransformMatrix)},

        {"native_OnSurfaceCreated", "()V",      (void *) (native_OnSurfaceCreated)},
        {"native_OnSurfaceChanged", "(II)V",    (void *) (native_OnSurfaceChanged)},
        {"native_OnDrawFrame",      "()V",      (void *) (native_OnDrawFrame)},
        {"native_UpdateEyeOffset",      "(F)V",      (void *) (native_UpdateEyeOffset)},

};


static JNINativeMethod g_BgRenderMethods[] = {
        {"native_EglRenderInit",         "()V",     (void *) (native_EglRenderInit)},
        {"native_EglRenderSetImageData", "([BII)V", (void *) (native_EglRenderSetImageData)},
        {"native_EglRenderSetIntParams", "(II)V",   (void *) (native_EglRenderSetIntParams)},
        {"native_EglRenderDraw",         "()V",     (void *) (native_EglRenderDraw)},
        {"native_EglRenderUnInit",       "()V",     (void *) (natuve_BgRenderUnInit)},
};


static int
RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int methodNum) {
    if (env == nullptr) {
        LOGCATE("RegisterNativeMethods fail. env == NULL");
        return JNI_FALSE;
    }
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        LOGCATE("RegisterNativeMethods fail. clazz == NULL");
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, methodNum) < 0) {
        LOGCATE("RegisterNativeMethods fail.");
        return JNI_FALSE;
    }
    return JNI_TRUE;
}


void UnregisterNativeMethods(JNIEnv *pEnv, const char *clsName) {
    if (pEnv == nullptr) {
        LOGCATE("UnregisterNativeMethods fail. pEnv == NULL");
        return;
    }
    jclass clazz = pEnv->FindClass(clsName);
    if (clazz == nullptr) {
        LOGCATE("UnregisterNativeMethods fail. clazz == NULL");
        return;
    }
    pEnv->UnregisterNatives(clazz);
}

/**
 * 注册 Java 层声明的native 函数
 */
extern "C" jint JNI_OnLoad(JavaVM *javaVm, void *p) {
    jint jni_result = JNI_ERR;
    JNIEnv *env = nullptr;
    if (javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return jni_result;
    }
    jint register_result = RegisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME, g_RenderMethods,
                                                 sizeof(g_RenderMethods) /
                                                 sizeof(g_RenderMethods[0]));
    if (register_result != JNI_TRUE) {
        return jni_result;
    }
    register_result = RegisterNativeMethods(env,NATIVE_BG_RENDER_CLASS_NAME,g_BgRenderMethods,
                                            sizeof(g_BgRenderMethods)/ sizeof(g_BgRenderMethods[0]));

    if(register_result!=JNI_TRUE){
        return JNI_ERR;
    }


    return JNI_VERSION_1_6;
}

extern "C" void JNI_OnUnload(JavaVM *javaVm, void *p) {
    JNIEnv *env = nullptr;
    if (javaVm->GetEnv((void **) env, JNI_VERSION_1_6) != JNI_OK) {
        return;
    }
    UnregisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME);
    UnregisterNativeMethods(env, NATIVE_BG_RENDER_CLASS_NAME);
}

