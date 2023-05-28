package com.ya.opengl.studyopengl30.activity;

import static com.ya.opengl.studyopengl30.MyGLSurfaceView.IMAGE_FORMAT_RGBA;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_TEXTURE_MAP;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;

import com.ya.opengl.studyopengl30.MyGLSurfaceView;
import com.ya.opengl.studyopengl30.R;
import com.ya.opengl.studyopengl30.render.GLRender;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

/**
 * 纹理映射：https://blog.csdn.net/Kennethdroid/article/details/96887637
 */
public class TextureMappingActivity extends AppCompatActivity {
    private static final String TAG = "DrawTriangleActivity : ";

    private MyGLSurfaceView myGLSurfaceView;
    private GLRender        glRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate: ");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_texture_mapping);
        myGLSurfaceView = findViewById(R.id.mgl_texture_mapping);

        glRender = new GLRender();
        glRender.init();

        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_TEXTURE_MAP, 0);
        myGLSurfaceView.setNativeRender(glRender.getNativeRender());
        loadRGBAImage(R.mipmap.dzzz);

        myGLSurfaceView.requestRender();
    }


    private Bitmap loadRGBAImage(int resId) {
        InputStream is     = this.getResources().openRawResource(resId);
        Bitmap      bitmap = null;
        try {
            bitmap = BitmapFactory.decodeStream(is);
            if (bitmap != null) {
                int        bytes = bitmap.getByteCount();
                ByteBuffer buf   = ByteBuffer.allocate(bytes);
                bitmap.copyPixelsToBuffer(buf);
                byte[] byteArray = buf.array();
                glRender.setImageData(IMAGE_FORMAT_RGBA, bitmap.getWidth(), bitmap.getHeight(), byteArray);
            }
        } catch (Exception e) {
            e.printStackTrace();
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
    protected void onDestroy() {
        super.onDestroy();
        glRender.unInit();

    }
}