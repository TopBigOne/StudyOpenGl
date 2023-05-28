package com.ya.opengl.studyopengl30.activity;

import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_TRIANGLE;

import android.os.Bundle;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

import com.ya.opengl.studyopengl30.MyGLSurfaceView;
import com.ya.opengl.studyopengl30.R;
import com.ya.opengl.studyopengl30.render.GLRender;

/**
 * 画一个三角形
 */
public class DrawTriangleActivity extends AppCompatActivity {
    private static final String TAG = "DrawTriangleActivity : ";

    private MyGLSurfaceView myGLSurfaceView;

    private GLRender glRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate: ");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_draw_triangle);

        myGLSurfaceView = findViewById(R.id.mgl_triangle);
        glRender = new GLRender();
        glRender.init();
        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_TRIANGLE, 0);
        myGLSurfaceView.setNativeRender(glRender.getNativeRender());
        myGLSurfaceView.requestRender();
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        glRender.unInit();

    }
}