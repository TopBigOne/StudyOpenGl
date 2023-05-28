package com.ya.opengl.studyopengl30;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import com.ya.opengl.studyopengl30.render.NativeRender;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * @author :Top
 * @version :
 * @Date :  2023/5/25 19:07
 * @Desc :
 */
public class MyGLSurfaceView extends GLSurfaceView {

    private static final String TAG               = "MyGLSurfaceView:";
    public static final  int    IMAGE_FORMAT_RGBA = 0x01;
    public static final  int    IMAGE_FORMAT_NV21 = 0x02;
    public static final  int    IMAGE_FORMAT_NV12 = 0x03;
    public static final  int    IMAGE_FORMAT_I420 = 0x04;

    private NativeRender mNativeRender;

    public MyGLSurfaceView(Context context) {
        this(context, null);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        setRenderer(renderer);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }


    private GLSurfaceView.Renderer renderer = new android.opengl.GLSurfaceView.Renderer() {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            if (mNativeRender == null) {
                return;
            }
            mNativeRender.native_OnSurfaceCreated();

        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            if (mNativeRender == null) {
                return;
            }
            mNativeRender.native_OnSurfaceChanged(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            if (mNativeRender == null) {
                return;
            }
            mNativeRender.native_OnDrawFrame();
        }
    };


    public void setNativeRender(NativeRender nativeRender) {
        mNativeRender = nativeRender;

    }
}
