package com.ya.opengl.studyopengl30;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.ya.opengl.studyopengl30.activity.DrawTriangleActivity;
import com.ya.opengl.studyopengl30.activity.TextureMappingActivity;
import com.ya.opengl.studyopengl30.activity.YUVRenderActivity;
import com.ya.opengl.studyopengl30.render.GLRender;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    Button btnDrawTriangle;
    Button btnTextureMapping;
    Button btnYUVRender;
    private static final String TAG = "MainActivity: ";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();
        initEventListener();
    }


    private void initView() {
        btnDrawTriangle = findViewById(R.id.btn_draw_triangle);
        btnTextureMapping = findViewById(R.id.btn_texture_mapping);
        btnYUVRender = findViewById(R.id.btn_yuv_render);
    }

    private void initEventListener() {
        btnDrawTriangle.setOnClickListener(this);
        btnTextureMapping.setOnClickListener(this);
        btnYUVRender.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_draw_triangle:
                toAnotherActivity(DrawTriangleActivity.class);
                break;
            case R.id.btn_texture_mapping:
                toAnotherActivity(TextureMappingActivity.class);
                break;
            case R.id.btn_yuv_render:
                toAnotherActivity(YUVRenderActivity.class);
                break;
            default:
        }
    }

    private void toAnotherActivity(Class<?> cls) {
        Log.i(TAG, "toAnotherActivity: target activity name is : " + cls.getSimpleName());
        Intent intent = new Intent(MainActivity.this, cls);
        startActivity(intent);
    }
}