package com.ya.opengl.studyopengl30.activity;

import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_KEY_VISUALIZE_AUDIO;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_PBO;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;

import com.ya.opengl.audio.AudioCollector;
import com.ya.opengl.studyopengl30.MainActivity;
import com.ya.opengl.studyopengl30.MyGLSurfaceView;
import com.ya.opengl.studyopengl30.R;
import com.ya.opengl.studyopengl30.render.GLRender;

public class VisualizeAudioActivity extends AppCompatActivity {

    private static final String TAG = "VisualizeAudioActivity : ";
    private AudioCollector mAudioCollector;

    private MyGLSurfaceView myGLSurfaceView;
    private GLRender        glRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate: ");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_visualize_audio);

        myGLSurfaceView = findViewById(R.id.mgl_visualize_audio_render);
        glRender = new GLRender();
        glRender.init();

        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_KEY_VISUALIZE_AUDIO, 0);

        mAudioCollector = new AudioCollector(VisualizeAudioActivity.this);
        mAudioCollector.addCallback(audioCollectorCallback);
        mAudioCollector.init();

        myGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mAudioCollector != null) {
            mAudioCollector.unInit();
            mAudioCollector = null;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mAudioCollector != null) {
            mAudioCollector.unInit();
            mAudioCollector = null;
        }
    }

    private final AudioCollector.AudioCollectorCallback audioCollectorCallback = new AudioCollector.AudioCollectorCallback() {
        @Override
        public void onAudioBufferCallback(short[] buffer) {
            if (glRender!=null) {
                Log.i(TAG, "onAudioBufferCallback: setAudioData");
                glRender.setAudioData(buffer);
            }
        }
    };
}