package com.ya.opengl.studyopengl30;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ScrollView;

import com.ya.opengl.studyopengl30.activity.*;
import com.ya.opengl.studyopengl30.egl.EGLActivity;


import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static {
        System.loadLibrary("studyopengl30");
    }

    private static final String TAG = "MainActivity: ";

    ScrollView scrollView;

    Button btnDrawTriangle;
    Button btnTextureMapping;
    Button btnYUVRender;
    Button btnVAOVBORender;
    Button btnFBORender;
    Button btnEGLRender;
    Button btnTransformFeedbackRender;
    Button btnMultiLightsRender;
    Button btnCoordSystemRender;
    Button btnBasicLightingRender;
    Button btnDepthTestRender;
    Button btnStencilTestRender;
    Button btnBlendingRender;
    Button btnInstancingRender;
    Button btnParticlesRender;
    Button btnPBORender;
    Button btnMRTRender;
    Button btnFBOBlitRender;
    Button btnSkyBoxRender;
    Button btnShockWaveRender;
    Button btnAvatarRender;
    Button btnScratchCardRender;
    Button btnVisualizeAudioRender;
    Button btnRotaryHeadRender;
    Button btnBigEyesRender;


    List<Button> buttonList = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();
        initEventListener();
    }


    private void initView() {
        scrollView = findViewById(R.id.sv_main);

        btnDrawTriangle = findViewById(R.id.btn_draw_triangle);
        btnTextureMapping = findViewById(R.id.btn_texture_mapping);
        btnYUVRender = findViewById(R.id.btn_yuv_render);
        btnVAOVBORender = findViewById(R.id.btn_avo_vbo_render);
        btnFBORender = findViewById(R.id.btn_fbo_render);
        btnEGLRender = findViewById(R.id.btn_egl_render);
        btnTransformFeedbackRender = findViewById(R.id.btn_transform_feedback_render);
        btnMultiLightsRender = findViewById(R.id.btn_multi_lights_render);
        btnCoordSystemRender = findViewById(R.id.btn_coord_system_render);
        btnBasicLightingRender = findViewById(R.id.btn_basic_lighting_render);

        btnDepthTestRender = findViewById(R.id.btn_depth_test_render);
        btnStencilTestRender = findViewById(R.id.btn_stencil_test_render);
        btnBlendingRender = findViewById(R.id.btn_blending_test_render);
        btnInstancingRender = findViewById(R.id.btn_instancing_render);
        btnParticlesRender = findViewById(R.id.btn_particles_render);
        btnPBORender = findViewById(R.id.btn_pbo_render);
        btnMRTRender = findViewById(R.id.btn_mrt_render);
        btnFBOBlitRender = findViewById(R.id.btn_fbo_blit_render);
        btnSkyBoxRender = findViewById(R.id.btn_sky_box_render);
        btnShockWaveRender = findViewById(R.id.btn_shock_wave_render);
        btnAvatarRender = findViewById(R.id.btn_avatar_render);
        btnScratchCardRender = findViewById(R.id.btn_scratch_card_render);
        btnVisualizeAudioRender = findViewById(R.id.btn_visualize_audio_render);
        btnRotaryHeadRender = findViewById(R.id.btn_rotary_head_render);
        btnBigEyesRender = findViewById(R.id.btn_big_eye_render);


        buttonList.add(btnDrawTriangle);
        buttonList.add(btnTextureMapping);
        buttonList.add(btnYUVRender);
        buttonList.add(btnVAOVBORender);
        buttonList.add(btnFBORender);
        buttonList.add(btnEGLRender);
        buttonList.add(btnTransformFeedbackRender);
        buttonList.add(btnMultiLightsRender);
        buttonList.add(btnCoordSystemRender);
        buttonList.add(btnBasicLightingRender);
        buttonList.add(btnDepthTestRender);
        buttonList.add(btnStencilTestRender);
        buttonList.add(btnBlendingRender);
        buttonList.add(btnInstancingRender);
        buttonList.add(btnParticlesRender);
        buttonList.add(btnPBORender);
        buttonList.add(btnMRTRender);
        buttonList.add(btnFBOBlitRender);
        buttonList.add(btnSkyBoxRender);
        buttonList.add(btnShockWaveRender);
        buttonList.add(btnAvatarRender);
        buttonList.add(btnScratchCardRender);
        buttonList.add(btnVisualizeAudioRender);
        buttonList.add(btnRotaryHeadRender);
        buttonList.add(btnBigEyesRender);


    }

    private void initEventListener() {
        for (Button button : buttonList) {
            button.setOnClickListener(this);
        }
    }


    @Override
    protected void onResume() {
        super.onResume();
        scrollView.post(new Runnable() {
            @Override
            public void run() {
                scrollView.smoothScrollTo(0, scrollView.getBottom());
            }
        });

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
            case R.id.btn_coord_system_render:
                toAnotherActivity(CoordSystemActivity.class);
                break;
            case R.id.btn_basic_lighting_render:
                // 光照基础
                toAnotherActivity(BasicLightingActivity.class);
                break;
            case R.id.btn_depth_test_render:
                // 深度测试
                toAnotherActivity(DepthTestingActivity.class);
                break;
            case R.id.btn_stencil_test_render:
                // 深度测试
                toAnotherActivity(StencilTestingActivity.class);
                break;
            case R.id.btn_blending_test_render:
                // 深度测试
                toAnotherActivity(BlendingActivity.class);
                break;
            case R.id.btn_instancing_render:
                // 实例化
                toAnotherActivity(InstancingActivity.class);
                break;
            case R.id.btn_particles_render:
                // 粒子
                toAnotherActivity(ParticlesActivity.class);
                break;
            case R.id.btn_pbo_render:
                // pbo:
                toAnotherActivity(PBOActivity.class);
                break;
            case R.id.btn_mrt_render:
                // pbo:
                toAnotherActivity(MRTActivity.class);
                break;
            case R.id.btn_fbo_blit_render:
                // pbo:
                toAnotherActivity(FboBlitActivity.class);
                break;
            case R.id.btn_sky_box_render:
                // texture_cube_map:
                toAnotherActivity(SkyBoxActivity.class);
                break;

            case R.id.btn_shock_wave_render:
                // 水波纹:
                toAnotherActivity(ShockWaveActivity.class);
                break;
            case R.id.btn_avatar_render:
                // 水波纹:
                toAnotherActivity(AvatarActivity.class);
                break;
            case R.id.btn_scratch_card_render:
                // 水波纹:
                toAnotherActivity(ScratchCardActivity.class);
                break;
            case R.id.btn_visualize_audio_render:
                // audio 可视化
                toAnotherActivity(VisualizeAudioActivity.class);
                break;

            case R.id.btn_rotary_head_render:
                // 头部晃动
                toAnotherActivity(RotaryHeadActivity.class);
                break;

            case R.id.btn_big_eye_render:
                // 头部晃动
                toAnotherActivity(BigEyesActivity.class);
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