package com.ya.opengl.studyopengl30.activity;

import static com.ya.opengl.studyopengl30.MyGLSurfaceView.IMAGE_FORMAT_RGBA;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_PBO;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_SKYBOX;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;

import com.ya.opengl.studyopengl30.MyGLSurfaceView;
import com.ya.opengl.studyopengl30.R;
import com.ya.opengl.studyopengl30.render.GLRender;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class SkyBoxActivity extends AppCompatActivity {

    private MyGLSurfaceView myGLSurfaceView;
    private GLRender        glRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sky_box);


        myGLSurfaceView = findViewById(R.id.mgl_sky_box_render);
        glRender = new GLRender();
        glRender.init();

        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_SKYBOX, 0);
        myGLSurfaceView.setNativeRender(glRender.getNativeRender());
        loadRGBAImage(R.mipmap.right, 0);
        loadRGBAImage(R.mipmap.left, 1);
        loadRGBAImage(R.mipmap.top, 2);
        loadRGBAImage(R.mipmap.bottom, 3);
        loadRGBAImage(R.mipmap.back, 4);
        loadRGBAImage(R.mipmap.front, 5);

        myGLSurfaceView.requestRender();
    }


    private Bitmap loadRGBAImage(int resId, int index) {
        InputStream is = this.getResources().openRawResource(resId);
        Bitmap      bitmap;
        try {
            bitmap = BitmapFactory.decodeStream(is);
            if (bitmap != null) {
                int        bytes = bitmap.getByteCount();
                ByteBuffer buf   = ByteBuffer.allocate(bytes);
                bitmap.copyPixelsToBuffer(buf);
                byte[] byteArray = buf.array();
                glRender.setImageDataWithIndex(index, IMAGE_FORMAT_RGBA, bitmap.getWidth(), bitmap.getHeight(), byteArray);
            }
        } finally {
            try {
                is.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return bitmap;
    }
}