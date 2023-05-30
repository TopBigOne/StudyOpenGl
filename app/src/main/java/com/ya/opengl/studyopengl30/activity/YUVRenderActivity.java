package com.ya.opengl.studyopengl30.activity;

import static com.ya.opengl.studyopengl30.MyGLSurfaceView.IMAGE_FORMAT_NV21;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_TEXTURE_MAP;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_YUV_TEXTURE_MAP;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.ya.opengl.studyopengl30.MyGLSurfaceView;
import com.ya.opengl.studyopengl30.R;
import com.ya.opengl.studyopengl30.render.GLRender;

import java.io.IOException;
import java.io.InputStream;

public class YUVRenderActivity extends AppCompatActivity {

    private MyGLSurfaceView myGLSurfaceView;
    private GLRender        glRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_yuv_render);

        myGLSurfaceView = findViewById(R.id.mgl_yuv_render);

        glRender = new GLRender();
        glRender.init();

        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_YUV_TEXTURE_MAP, 0);

        myGLSurfaceView.setNativeRender(glRender.getNativeRender());
        loadNV21Image();

        myGLSurfaceView.requestRender();

    }


    private void loadNV21Image() {
        InputStream is = null;
        try {
            is = getAssets().open("YUV_Image_840x1074.NV21");
            int length = 0;
            length = is.available();
            byte[] buffer = new byte[length];
            is.read(buffer);
            glRender.setImageData(IMAGE_FORMAT_NV21, 840, 1074, buffer);


        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                is.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


    }
}