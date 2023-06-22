package com.ya.opengl.studyopengl30.activity;

import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_BLENDING;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_KEY_BEZIER_CURVE;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.ya.opengl.studyopengl30.MyGLSurfaceView;
import com.ya.opengl.studyopengl30.R;
import com.ya.opengl.studyopengl30.render.GLRender;

/**
 * C++ 层看 贝塞尔算法。
 */
public class BezierCurveActivity extends AppCompatActivity {
    private MyGLSurfaceView myGLSurfaceView;
    private GLRender        glRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bezier_curve);

        myGLSurfaceView = findViewById(R.id.mgl_bezier_curve_render);
        myGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);
        glRender = new GLRender();
        glRender.init();
        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_KEY_BEZIER_CURVE, 0);
        myGLSurfaceView.setNativeRender(glRender.getNativeRender());
    }
}