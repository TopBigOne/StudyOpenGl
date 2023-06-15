package com.ya.opengl.studyopengl30;

import static com.ya.opengl.studyopengl30.render.NativeRender.*;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
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


    private int mRatioWidth  = 0;
    private int mRatioHeight = 0;

    public MyGLSurfaceView(Context context) {
        this(context, null);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        //最后 2 个参数表示分别配置 16 位的深度缓冲区和模板缓冲区
        setEGLConfigChooser(8, 8, 8, 8, 16, 8);
        setRenderer(renderer);
        setRenderMode(RENDERMODE_WHEN_DIRTY);

        mScaleGestureDetector = new ScaleGestureDetector(context, scaleGestureListener);
    }

    public void setAspectRatio(int width, int height) {
        Log.d(TAG, "setAspectRatio() called with: width = [" + width + "], height = [" + height + "]");
        if (width < 0 || height < 0) {
            throw new IllegalArgumentException("Size cannot be negative.");
        }

        mRatioWidth = width;
        mRatioHeight = height;
        requestLayout();
    }


    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        int width  = MeasureSpec.getSize(widthMeasureSpec);
        int height = MeasureSpec.getSize(heightMeasureSpec);

        if (0 == mRatioWidth || 0 == mRatioHeight) {
            setMeasuredDimension(width, height);
        } else {
            if (width < height * mRatioWidth / mRatioHeight) {
                setMeasuredDimension(width, width * mRatioHeight / mRatioWidth);
            } else {
                setMeasuredDimension(height * mRatioWidth / mRatioHeight, height);
            }
        }
    }


    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getPointerCount() == 1) {
            consumeTouchEvent(event);
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

    private void consumeTouchEvent(MotionEvent e) {
        dealClickEvent(e);
        float touchX = -1, touchY = -1;
        switch (e.getAction()) {
            case MotionEvent.ACTION_MOVE:
                touchX = e.getX();
                touchY = e.getY();
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_CANCEL:
                touchX = -1;
                touchY = -1;
                break;
            default:
                break;
        }

        //滑动、触摸
        switch (mNativeRender.getSampleType()) {
            case SAMPLE_TYPE_KEY_SCRATCH_CARD:
                mNativeRender.setTouchLoc(touchX, touchY);
                requestRender();
                break;
            default:
                break;
        }

        //点击
        switch (e.getAction()) {
            case MotionEvent.ACTION_MOVE:
                //touchX = e.getX();
                //touchY = e.getY();
                break;
            case MotionEvent.ACTION_UP:
                break;
            case MotionEvent.ACTION_CANCEL:
                break;
            default:
                break;
        }

    }


    public void dealClickEvent(MotionEvent e) {
        float touchX = -1, touchY = -1;


        switch (e.getAction()) {
            case MotionEvent.ACTION_UP:
                touchX = e.getX();
                touchY = e.getY();
            {
                //点击
                switch (mNativeRender.getSampleType()) {
                    case SAMPLE_TYPE_KEY_SHOCK_WAVE:
                        mNativeRender.setTouchLoc(touchX, touchY);
                        break;
                    default:
                        break;
                }
            }
            break;
            default:
                break;
        }
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
            switch (mNativeRender.getSampleType()) {
                case SAMPLE_TYPE_COORD_SYSTEM:
                case SAMPLE_TYPE_BASIC_LIGHTING:
                case SAMPLE_TYPE_INSTANCING:
                case SAMPLE_TYPE_3D_MODEL:
                case SAMPLE_TYPE_KEY_VISUALIZE_AUDIO:
                case SAMPLE_TYPE_KEY_TEXT_RENDER: {
                    float preSpan = detector.getPreviousSpan();
                    float curSpan = detector.getCurrentSpan();
                    if (curSpan < preSpan) {
                        mCurScale = mPreScale - (preSpan - curSpan) / 200;
                    } else {
                        mCurScale = mPreScale + (curSpan - preSpan) / 200;
                    }
                    mCurScale = Math.max(0.05f, Math.min(mCurScale, 80.0f));
                    mNativeRender.updateTransformMatrix(mXAngle, mYAngle, mCurScale, mCurScale);
                    requestRender();
                }
                break;
                default:
                    break;
            }

            return false;
        }

        @Override
        public boolean onScaleBegin(ScaleGestureDetector detector) {
            return true;
        }

        @Override
        public void onScaleEnd(ScaleGestureDetector detector) {
            mPreScale = mCurScale;
            mLastMultiTouchTime = System.currentTimeMillis();
        }
    };
}
