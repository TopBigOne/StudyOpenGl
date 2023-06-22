package com.ya.opengl.studyopengl30.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.Nullable;

/**
 * @author : top
 * @version :
 * @Date :  2023/6/22 16:04
 * @Desc : 三次贝塞尔曲线 画圆.
 */
public class BezierCircleViewThree extends View {
    private static final String TAG = "BezierCircleViewThree : ";

    private Paint mPaint;
    private Path  mPath;


    public BezierCircleViewThree(Context context) {
        this(context, null);

    }

    public BezierCircleViewThree(Context context, @Nullable AttributeSet attrs) {

        this(context, null, 0);
    }

    public BezierCircleViewThree(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        this(context, null, 0, 0);

    }

    public BezierCircleViewThree(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init();
    }


    private void init() {
        mPaint = new Paint();
        mPaint.setColor(Color.RED);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeWidth(5);

        mPath = new Path();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        int centerX = (getWidth() >> 2) + 100;
        int centerY = getHeight() >> 2;
        int radius  = 300;

        mPath.reset();
        mPath.moveTo(centerX + radius, centerY);

        //
        float c = (float) (radius * 0.5522847498);
        // ，用于绘制三次贝塞尔曲线。
        mPath.cubicTo(centerX + radius, centerY + c, centerX + c, centerY + radius, centerX, centerY + radius);
        mPath.cubicTo(centerX - c, centerY + radius, centerX - radius, centerY + c, centerX - radius, centerY);
        mPath.cubicTo(centerX - radius, centerY - c, centerX - c, centerY - radius, centerX, centerY - radius);
        mPath.cubicTo(centerX + c, centerY - radius, centerX + radius, centerY - c, centerX + radius, centerY);

        canvas.drawPath(mPath, mPaint);
    }

}
