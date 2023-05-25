package com.ya.opengl.studyopengl30.render;

/**
 * @author :top
 * @version :
 * @Date :  2023/5/25 15:05
 * @Desc :
 */
public class NativeRender {

    public native void native_Init();

    public native void native_UnInit();

    public native void native_SetImageData(int format, int width, int height, byte[] bytes);

    public native void native_OnSurfaceCreated();

    public native void native_OnSurfaceChanged(int width, int height);

    public native void native_OnDrawFrame();
}
