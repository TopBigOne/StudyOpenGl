package com.ya.opengl.studyopengl30;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.ya.opengl.studyopengl30.render.GLRender;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        GLRender glRender = new GLRender();
        glRender.init();
    }
}