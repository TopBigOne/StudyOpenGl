package com.ya.opengl.studyopengl30;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.ya.opengl.studyopengl30.activity.AVO_VBO_Activity;
import com.ya.opengl.studyopengl30.activity.DrawTriangleActivity;
import com.ya.opengl.studyopengl30.activity.FBOActivity;
import com.ya.opengl.studyopengl30.activity.MultiLightsActivity;
import com.ya.opengl.studyopengl30.activity.TextureMappingActivity;
import com.ya.opengl.studyopengl30.activity.TransformFeedbackActivity;
import com.ya.opengl.studyopengl30.activity.YUVRenderActivity;
import com.ya.opengl.studyopengl30.egl.EGLActivity;
import com.ya.opengl.studyopengl30.render.GLRender;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static {
        System.loadLibrary("studyopengl30");
    }

    private static final String TAG = "MainActivity: ";

    Button       btnDrawTriangle;
    Button       btnTextureMapping;
    Button       btnYUVRender;
    Button       btnVAOVBORender;
    Button       btnFBORender;
    Button       btnEGLRender;
    Button       btnTransformFeedbackRender;
    Button       btnMultiLightsRender;
    List<Button> buttonList = new ArrayList<>();

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
        btnVAOVBORender = findViewById(R.id.btn_avo_vbo_render);
        btnFBORender = findViewById(R.id.btn_fbo_render);
        btnEGLRender = findViewById(R.id.btn_egl_render);
        btnTransformFeedbackRender = findViewById(R.id.btn_transform_feedback_render);
        btnMultiLightsRender = findViewById(R.id.btn_multi_lights_render);

        buttonList.add(btnDrawTriangle);
        buttonList.add(btnTextureMapping);
        buttonList.add(btnYUVRender);
        buttonList.add(btnVAOVBORender);
        buttonList.add(btnFBORender);
        buttonList.add(btnEGLRender);
        buttonList.add(btnTransformFeedbackRender);
        buttonList.add(btnMultiLightsRender);

    }

    private void initEventListener() {
        for (Button button : buttonList) {
            button.setOnClickListener(this);
        }
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

            case R.id.btn_avo_vbo_render:
                toAnotherActivity(AVO_VBO_Activity.class);
                break;
            case R.id.btn_fbo_render:
                toAnotherActivity(FBOActivity.class);
                break;
            case R.id.btn_egl_render:
                toAnotherActivity(EGLActivity.class);

            case R.id.btn_multi_lights_render:
                toAnotherActivity(MultiLightsActivity.class);
                break;
            case R.id.btn_transform_feedback_render:
                toAnotherActivity(TransformFeedbackActivity.class);
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