package com.ya.opengl.studyopengl30.activity;

import static android.opengl.GLSurfaceView.RENDERMODE_WHEN_DIRTY;
import static com.ya.opengl.studyopengl30.MyGLSurfaceView.IMAGE_FORMAT_RGBA;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_KEY_BIG_EYES;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.DialogTitle;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.widget.ProgressBar;
import android.widget.SeekBar;

import com.ya.opengl.studyopengl30.MyGLSurfaceView;
import com.ya.opengl.studyopengl30.R;
import com.ya.opengl.studyopengl30.render.GLRender;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class BigEyesActivity extends AppCompatActivity {

    private static final String TAG = "BigEyesActivity : ";

    private MyGLSurfaceView myGLSurfaceView;
    private GLRender        glRender;
    private SeekBar         progressBar;
    private float           bigScale = 0.0f;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_big_eyes);

        myGLSurfaceView = findViewById(R.id.mgl_big_eyes_render);
        progressBar = findViewById(R.id.sk_big_eyes);


        glRender = new GLRender();
        glRender.init();

        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_KEY_BIG_EYES, 0);

        myGLSurfaceView.setNativeRender(glRender.getNativeRender());
        Bitmap bitmap = loadRGBAImage(R.mipmap.yifei);
        myGLSurfaceView.setAspectRatio(bitmap.getWidth(), bitmap.getHeight());
        myGLSurfaceView.setRenderMode(RENDERMODE_WHEN_DIRTY);
        myGLSurfaceView.requestRender();

        initProgressBar();
    }


    private void initProgressBar() {
        progressBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                bigScale = progress / 100.f;
                Log.d(TAG, "onProgressChanged: bigScale : " + bigScale);
                glRender.getNativeRender().native_UpdateEyeOffset(bigScale);
                myGLSurfaceView.requestRender();
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

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