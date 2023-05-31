package com.ya.opengl.studyopengl30.activity;

import static com.ya.opengl.studyopengl30.render.NativeRender.*;


import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.ya.opengl.studyopengl30.MyGLSurfaceView;
import com.ya.opengl.studyopengl30.R;
import com.ya.opengl.studyopengl30.render.GLRender;

public class AVO_VBO_Activity extends AppCompatActivity {
    private MyGLSurfaceView myGLSurfaceView;
    private GLRender        glRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_avo_vbo);


        myGLSurfaceView = findViewById(R.id.mgl_vao_abo_render);

        glRender = new GLRender();
        glRender.init();

        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_VAO, 0);

        myGLSurfaceView.setNativeRender(glRender.getNativeRender());

        myGLSurfaceView.requestRender();
    }
}