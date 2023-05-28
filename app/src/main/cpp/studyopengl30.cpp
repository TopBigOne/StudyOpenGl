#include <jni.h>

#include "util/LogUtils.h"
#include "render/MyGLRenderContext.h"

#define NATIVE_RENDER_CLASS_NAME "com/ya/opengl/studyopengl30/render/NativeRender"


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

static JNINativeMethod g_RenderMethods[] = {
        {"native_Init",             "()V",      (void *) (native_Init)},
        {"native_UnInit",           "()V",      (void *) (native_UnInit)},
        {"native_SetImageData",     "(III[B)V", (void *) (native_SetImageData)},

//        {"native_SetImageDataWithIndex", "(IIII[B)V", (void *) (native_SetImageDataWithIndex)},
        {"native_SetParamsInt",     "(III)V",   (void *) (native_SetParamsInt)},
//        {"native_SetParamsFloat", "(IFF)V", (void *) (native_SetParamsFloat)},
//        {"native_SetAudioData", "([S)V", (void *) (native_SetAudioData)},
//        {"native_UpdateTransformMatrix", "(FFFF)V", (void *) (native_UpdateTransformMatrix)},

        {"native_OnSurfaceCreated", "()V",      (void *) (native_OnSurfaceCreated)},
        {"native_OnSurfaceChanged", "(II)V",    (void *) (native_OnSurfaceChanged)},
        {"native_OnDrawFrame",      "()V",      (void *) (native_OnDrawFrame)},

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
    return JNI_VERSION_1_6;
}

extern "C" void JNI_OnUnload(JavaVM *javaVm, void *p) {
    JNIEnv *env = nullptr;
    if (javaVm->GetEnv((void **) env, JNI_VERSION_1_6) != JNI_OK) {
        return;
    }
    UnregisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME);
}

