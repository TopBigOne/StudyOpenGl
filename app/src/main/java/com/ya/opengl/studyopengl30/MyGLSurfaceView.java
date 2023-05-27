package com.ya.opengl.studyopengl30;

import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import com.ya.opengl.studyopengl30.render.GLRender;
import com.ya.opengl.studyopengl30.render.NativeRender;

/**
 * @author :Top
 * @version :
 * @Date :  2023/5/25 19:07
 * @Desc :
 */
public class MyGLSurfaceView extends GLSurfaceView {

    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    public static final int IMAGE_FORMAT_I420 = 0x04;

    private NativeRender nativeRender;
    private GLRender     mGLRender;

    public MyGLSurfaceView(Context context) {
        this(context, null);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        mGLRender = new GLRender();

        mGLRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE, 0);
        setRenderer(mGLRender.getRenderer());
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }


}
