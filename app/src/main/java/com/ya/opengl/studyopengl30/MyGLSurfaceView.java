package com.ya.opengl.studyopengl30;

import static com.ya.opengl.studyopengl30.render.NativeRender.*;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

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

    private static final String TAG = "MyGLSurfaceView:";

    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    public static final int IMAGE_FORMAT_I420 = 0x04;

    /**
     * 触摸缩放因子
     */
    private static final float TOUCH_SCALE_FACTOR = 180.0f / 320;

    private long  mLastMultiTouchTime;
    private float mPreviousY;
    private float mPreviousX;
    private int   mXAngle;
    private int   mYAngle;
    private float mPreScale = 1.0f;
    private float mCurScale = 1.0f;

    // 缩放手势
    private ScaleGestureDetector mScaleGestureDetector;
    private NativeRender         mNativeRender;

    public MyGLSurfaceView(Context context) {
        this(context, null);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        setRenderer(renderer);
        setRenderMode(RENDERMODE_WHEN_DIRTY);

        mScaleGestureDetector = new ScaleGestureDetector(context, scaleGestureListener);
    }


    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getPointerCount() == 1) {
            long currentTimeMillis = System.currentTimeMillis();
            if ((currentTimeMillis - mLastMultiTouchTime > 200)) {
                float x = event.getX();
                float y = event.getY();
                if (event.getAction() == MotionEvent.ACTION_MOVE) {
                    float dx = x - mPreviousX;
                    float dy = y - mPreviousY;
                    mYAngle += dx * TOUCH_SCALE_FACTOR;
                    mXAngle += dy * TOUCH_SCALE_FACTOR;
                }
                mPreviousY = y;
                mPreviousX = x;

                switch (mNativeRender.getSampleType()) {
                    case SAMPLE_TYPE_FBO_LEG:
                    case SAMPLE_TYPE_COORD_SYSTEM:
                    case SAMPLE_TYPE_BASIC_LIGHTING:
                    case SAMPLE_TYPE_TRANS_FEEDBACK:
                    case SAMPLE_TYPE_MULTI_LIGHTS:
                    case SAMPLE_TYPE_DEPTH_TESTING:
                    case SAMPLE_TYPE_INSTANCING:
                    case SAMPLE_TYPE_STENCIL_TESTING:
                    case SAMPLE_TYPE_PARTICLES:
                    case SAMPLE_TYPE_SKYBOX:
                    case SAMPLE_TYPE_3D_MODEL:
                    case SAMPLE_TYPE_PBO:
                    case SAMPLE_TYPE_KEY_VISUALIZE_AUDIO:
                    case SAMPLE_TYPE_KEY_UBO:
                    case SAMPLE_TYPE_KEY_TEXT_RENDER:
                        mNativeRender.updateTransformMatrix(mXAngle, mYAngle, mCurScale, mCurScale);
                        requestRender();
                        break;
                    default:
                        break;
                }
            }

        } else {
            // 多个手指
            mScaleGestureDetector.onTouchEvent(event);
        }
        //
        return true;
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

    private ScaleGestureDetector.OnScaleGestureListener scaleGestureListener = new ScaleGestureDetector.SimpleOnScaleGestureListener() {
        @Override
        public boolean onScale(ScaleGestureDetector detector) {
            return super.onScale(detector);
        }

        @Override
        public boolean onScaleBegin(ScaleGestureDetector detector) {
            return super.onScaleBegin(detector);
        }

        @Override
        public void onScaleEnd(ScaleGestureDetector detector) {
            super.onScaleEnd(detector);
        }
    };
}
