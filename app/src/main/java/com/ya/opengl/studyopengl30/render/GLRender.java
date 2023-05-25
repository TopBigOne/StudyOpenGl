package com.ya.opengl.studyopengl30.render;

import android.opengl.GLSurfaceView;
import android.os.IInterface;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * @author : 周广亚
 * @version :
 * @Date :  2023/5/25 15:04
 * @Desc :
 */
public class GLRender {
    private static final String TAG = "GLRender : ";

    private NativeRender mNativeRender;

    static {
        System.loadLibrary("studyopengl30");
    }

    public GLRender() {
        this.mNativeRender = new NativeRender();
    }

    public void init() {
        mNativeRender.native_Init();
    }

    public void unInit() {
        mNativeRender.native_UnInit();
    }

    public void setImageData(int format, int width, int height, byte[] bytes) {
        mNativeRender.native_SetImageData(format, width, height, bytes);
    }

    public GLSurfaceView.Renderer getRenderer() {
        return renderer;
    }

    private GLSurfaceView.Renderer renderer = new android.opengl.GLSurfaceView.Renderer() {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            mNativeRender.native_OnSurfaceCreated();

        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            mNativeRender.native_OnSurfaceChanged(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            mNativeRender.native_OnDrawFrame();
        }
    };


}
