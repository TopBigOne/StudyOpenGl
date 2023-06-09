package com.ya.opengl.studyopengl30.activity;

import static com.ya.opengl.studyopengl30.MyGLSurfaceView.IMAGE_FORMAT_RGBA;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_DEPTH_TESTING;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_STENCIL_TESTING;

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

public class StencilTestingActivity extends AppCompatActivity {

    private MyGLSurfaceView myGLSurfaceView;
    private  GLRender        glRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_stencil_testing);

        myGLSurfaceView = findViewById(R.id.mgl_stencil_test_render);


        glRender = new GLRender();
        glRender.init();

        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_STENCIL_TESTING, 0);
        myGLSurfaceView.setNativeRender(glRender.getNativeRender());
        loadRGBAImage(R.mipmap.board_texture);

        myGLSurfaceView.requestRender();
    }


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
        }
        finally
        {
            try
            {
                is.close();
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }
        }
        return bitmap;
    }
}