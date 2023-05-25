#include <jni.h>

#include "util/LogUtils.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_ya_opengl_studyopengl30_render_NativeRender_native_1Init(JNIEnv *env, jobject thiz) {
    LOGCATI("hello native.");


}
extern "C"
JNIEXPORT void JNICALL
Java_com_ya_opengl_studyopengl30_render_NativeRender_native_1UnInit(JNIEnv *env, jobject thiz) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_ya_opengl_studyopengl30_render_NativeRender_native_1SetImageData(JNIEnv *env, jobject thiz,
                                                                          jint format, jint width,
                                                                          jint height,
                                                                          jbyteArray bytes) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_ya_opengl_studyopengl30_render_NativeRender_native_1OnSurfaceCreated(JNIEnv *env,
                                                                              jobject thiz) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_ya_opengl_studyopengl30_render_NativeRender_native_1OnSurfaceChanged(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jint width,
                                                                              jint height) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_ya_opengl_studyopengl30_render_NativeRender_native_1OnDrawFrame(JNIEnv *env,
                                                                         jobject thiz) {


}