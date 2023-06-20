package com.ya.opengl.studyopengl30.activity;

import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;
import static com.ya.opengl.studyopengl30.MyGLSurfaceView.IMAGE_FORMAT_RGBA;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_KEY_BIG_HEAD;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_KEY_ROTARY_HEAD;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_PBO;

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

public class RotaryHeadActivity extends AppCompatActivity {


    private MyGLSurfaceView myGLSurfaceView;
    private GLRender        glRender;

    private Bitmap loadRGBAImage(int resId) {
        InputStream is = this.getResources().openRawResource(resId);
        Bitmap      bitmap;
        try {
            bitmap = BitmapFactory.decodeStream(is);
            if (bitmap != null) {
                int        bytes = bitmap.getByteCount();
                ByteBuffer buf   = ByteBuffer.allocate(bytes);
                bitmap.copyPixelsToBuffer(buf);
                byte[] byteArray = buf.array();
                glRender.setImageData(IMAGE_FORMAT_RGBA, bitmap.getWidth(), bitmap.getHeight(), byteArray);
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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rotary_head);

        myGLSurfaceView = findViewById(R.id.mgl_rotary_head_render);
        glRender = new GLRender();
        glRender.init();

        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_KEY_ROTARY_HEAD, 0);
        myGLSurfaceView.setNativeRender(glRender.getNativeRender());
        //        Bitmap b = loadRGBAImage(R.mipmap.img_2380);
        Bitmap b = loadRGBAImage(R.mipmap.huge);
        myGLSurfaceView.setAspectRatio(b.getWidth(), b.getHeight());
        myGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);
        myGLSurfaceView.requestRender();
    }
}