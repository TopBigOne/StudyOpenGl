package com.ya.opengl.studyopengl30.egl;

/**
 * @author : top
 * @version :
 * @Date :  2023/6/3 20:10
 * @Desc :
 */
public class NativeEglRender {
    public native void native_EglRenderInit();

    public native void native_EglRenderSetImageData(byte[] data, int width, int height);

    public native void native_EglRenderSetIntParams(int paramType, int param);

    public native void native_EglRenderDraw();

    public native void native_EglRenderUnInit();
}
