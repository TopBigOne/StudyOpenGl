package com.ya.opengl.studyopengl30.render;

import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;

import android.opengl.GLSurfaceView;
import android.os.IInterface;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * @author  :
 * @version :
 * @Date    :  2023/5/25 15:04
 * @Desc    :
 */
public class GLRender {
    private static final String TAG = "GLRender : ";

    private NativeRender mNativeRender;
    private int          mSampleType;

    public GLRender() {
        this.mNativeRender = new NativeRender();
    }

    public void init() {
        mNativeRender.native_Init();
    }

    public void unInit() {
        mNativeRender.native_UnInit();
    }

    public void setParamsInt(int paramType, int value0, int value1) {
        if (paramType == SAMPLE_TYPE) {
            mSampleType = value0;
        }
        mNativeRender.setSampleType(mSampleType);
        mNativeRender.native_SetParamsInt(paramType, value0, value1);
    }

    public void setImageData(int format, int width, int height, byte[] bytes) {
        mNativeRender.native_SetImageData(format, width, height, bytes);
    }

    public NativeRender getNativeRender() {
        return mNativeRender;
    }

}
