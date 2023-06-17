package com.ya.opengl.studyopengl30.activity;

import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;
import static com.ya.opengl.studyopengl30.MyGLSurfaceView.IMAGE_FORMAT_RGBA;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_KEY_AVATAR;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_KEY_BEATING_HEART;
import static com.ya.opengl.studyopengl30.render.NativeRender.SAMPLE_TYPE_KEY_SHOCK_WAVE;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.ya.opengl.studyopengl30.MyGLSurfaceView;
import com.ya.opengl.studyopengl30.R;
import com.ya.opengl.studyopengl30.render.GLRender;
import com.ya.opengl.util.CommonUtils;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

/**
 * 阿凡达：重力感应
 */
public class AvatarActivity extends AppCompatActivity {

    private              SensorManager mSensorManager;
    private static final String[]      REQUEST_PERMISSIONS = {Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO,};

    private static final int PERMISSION_REQUEST_CODE = 1;

    private MyGLSurfaceView myGLSurfaceView;
    private GLRender        glRender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_avatar);
        initSensor();

        myGLSurfaceView = findViewById(R.id.mgl_avatar_render);
        glRender = new GLRender();
        glRender.init();

        glRender.setParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_KEY_AVATAR, 0);
        myGLSurfaceView.setNativeRender(glRender.getNativeRender());

        // layer : 背景：蓝色背景萤火虫
        Bitmap b3 = loadRGBAImage(R.mipmap.avatar_a, 0);
        myGLSurfaceView.setAspectRatio(b3.getWidth(), b3.getHeight());
        // layer : 阿凡达头像
        loadRGBAImage(R.mipmap.avatar_b, 1);
        // layer : 白色萤火虫
        loadRGBAImage(R.mipmap.avatar_c, 2);
        myGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);
        myGLSurfaceView.requestRender();
    }


    private void initSensor() {
        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
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

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(sensorEventListener, mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY), SensorManager.SENSOR_DELAY_FASTEST);
        if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            Log.i(TAG, "onResume:  request permissions ");
            ActivityCompat.requestPermissions(this, REQUEST_PERMISSIONS, PERMISSION_REQUEST_CODE);
        }
        ///sdcard/Android/data/com.byteflow.app/files/Download
//        String fileDir = getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath();
//        CommonUtils.copyAssetsDirToSDCard(AvatarActivity.this, "poly", fileDir + "/model");
//        CommonUtils.copyAssetsDirToSDCard(AvatarActivity.this, "fonts", fileDir);
//        CommonUtils.copyAssetsDirToSDCard(AvatarActivity.this, "yuv", fileDir);

    }

    @Override
    protected void onPause() {
        super.onPause();
        mSensorManager.unregisterListener(sensorEventListener);
    }

    protected boolean hasPermissionsGranted(String[] permissions) {
        for (String permission : permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
                Toast.makeText(this, "We need the permission: WRITE_EXTERNAL_STORAGE", Toast.LENGTH_SHORT).show();
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }


    public SensorEventListener sensorEventListener = new SensorEventListener() {
        @Override
        public void onSensorChanged(SensorEvent event) {
            switch (event.sensor.getType()) {
                case Sensor.TYPE_GRAVITY:
                    Log.d(TAG, "onSensorChanged() called with TYPE_GRAVITY: [x,y,z] = [" + event.values[0] + ", " + event.values[1] + ", " + event.values[2] + "]");
                    if (mSampleSelectedIndex + SAMPLE_TYPE == SAMPLE_TYPE_KEY_AVATAR) {
                        glRender.getNativeRender().setGravityXY(event.values[0], event.values[1]);
                    }
                    break;
                default:
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {

        }
    };

    private int mSampleSelectedIndex = SAMPLE_TYPE_KEY_BEATING_HEART - SAMPLE_TYPE;

    private static final String TAG = "AvatarActivity : ";
}