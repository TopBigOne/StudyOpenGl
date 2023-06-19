package com.ya.opengl.audio;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.appcompat.widget.DialogTitle;
import androidx.core.app.ActivityCompat;

/**
 * @author : dev
 * @version :
 * @Date :  2023/6/19 16:07
 * @Desc : 音频收集
 */
public class AudioCollector implements AudioRecord.OnRecordPositionUpdateListener {
    private static final String TAG                   = "AudioCollector： ";
    private static final int    RECORDER_SAMPLE_RATE  = 44100;
    private static final int    RECORDER_CHANNELS     = 1;
    private static final int    RECORDER_ENCODING_BIT = 16;
    private static final int RECORDER_AUDIO_ENCODING = AudioFormat.ENCODING_PCM_16BIT;

    private AudioRecord mAudioRecord;
    private short[]     mAudioBuffer;
    private Handler     mHandler;
    private int         mBufferSize;

    private       AudioCollectorCallback mCallback;
    private final Context                mContext;


    public AudioCollector(Context context) {
        mContext = context;
        mBufferSize = AudioRecord.getMinBufferSize(RECORDER_SAMPLE_RATE, RECORDER_CHANNELS, RECORDER_AUDIO_ENCODING);
        mBufferSize = mBufferSize >> 1;
    }

    public void init() {
        if (ActivityCompat.checkSelfPermission(mContext, Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
            return;
        }
        Log.d(TAG, "init: ");

        mAudioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, RECORDER_SAMPLE_RATE, RECORDER_CHANNELS, RECORDER_AUDIO_ENCODING, mBufferSize);
        mAudioRecord.startRecording();

        // audioRecoder 将数据写到 mBufferSize 中.
        Thread mThread = new Thread("jar-Audio-Recorder-thread") {
            @Override
            public void run() {
                mAudioBuffer = new short[mBufferSize];
                Looper.prepare();
                mHandler = new Handler(Looper.myLooper());
                Log.d(TAG, "jar-Audio-Recorder : create Handler.");
                mAudioRecord.setRecordPositionUpdateListener(AudioCollector.this, mHandler);
                int   bytePerSample = RECORDER_ENCODING_BIT / 8;
                float sampleToDraw  = mBufferSize / bytePerSample;
                mAudioRecord.setPositionNotificationPeriod((int) sampleToDraw);
                // audioRecoder 将数据写到 mBufferSize 中.
                mAudioRecord.read(mAudioBuffer, 0, mBufferSize);
                Looper.loop();
            }
        };
        mThread.start();
    }

    public void addCallback(AudioCollectorCallback callback) {
        mCallback = callback;
    }


    /**
     * 用于在录音缓冲区达到指定标记位置时被调用。可以通过设置该监听器并实现该回调方法来实现对录音过程的控制和处理，
     * 例如: 控制录音停止、进行数据处理等操作。
     * 同时，该回调也可以用于实现录音进度条的更新或者其他需要根据录音位置信息进行处理的功能。
     *
     * @param recorder
     */
    @Override
    public void onMarkerReached(AudioRecord recorder) {
        Log.d(TAG, "onMarkerReached: ");

    }

    /**
     * 该方法会在录音缓冲区的位置发生变化时被调用。
     * 可以通过该回调来实现对录音过程的控制和处理，例如控制录音停止、进行数据处理等操作。
     * 同时，该回调也可以用于实现录音进度条的更新或者其他需要根据录音位置信息进行处理的功能。
     *
     * @param recorder
     */
    @Override
    public void onPeriodicNotification(AudioRecord recorder) {
        Log.d(TAG, "onPeriodicNotification: ");
        if (mAudioRecord == null) {
            return;
        }
        if (mCallback == null) {
            return;
        }
        if (mAudioRecord.getRecordingState() == AudioRecord.RECORDSTATE_RECORDING && mAudioRecord.read(mAudioBuffer, 0, mAudioBuffer.length) != 1) {
            Log.i(TAG, "    onPeriodicNotification: invoke onAudioBufferCallback ");
            mCallback.onAudioBufferCallback(mAudioBuffer);
        }
    }

    public void unInit() {
        if (mAudioRecord == null) {
            return;
        }
        mAudioRecord.stop();
        mAudioRecord.release();
        mHandler.getLooper().quitSafely();
        mAudioRecord = null;
    }


    public interface AudioCollectorCallback {
        void onAudioBufferCallback(short[] buffer); //little-endian
    }
}
