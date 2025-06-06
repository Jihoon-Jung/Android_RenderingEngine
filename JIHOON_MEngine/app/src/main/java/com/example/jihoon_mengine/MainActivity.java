package com.example.jihoon_mengine;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.Choreographer;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import com.example.jihoon_mengine.databinding.ActivityMainBinding;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'jihoon_mengine' library on application startup.
    static {
        System.loadLibrary("jihoon_mengine");
    }

    private ActivityMainBinding binding;

    private static final int REQUEST_PICK_IMAGE = 1001;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        SurfaceView surfaceView = findViewById(R.id.surfaceView);

        surfaceView.setFocusable(true);
        surfaceView.setFocusableInTouchMode(true);
        surfaceView.requestFocus(); // 포커스 강제 요청
        surfaceView.setClickable(true); // 클릭 가능하도록 설정

        SurfaceHolder holder = surfaceView.getHolder();
        holder.addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                if (nativeInit(holder.getSurface()))
                    Choreographer.getInstance().postFrameCallback(frameCallback);
                else
                    Log.d("ERROR", "EGL Initialize Error");
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                nativeOnSurfaceChanged(width, height); // JNI 호출
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                Log.d("JJH", "surfaceDestroyed");
                nativeRelease();
            }
        });

        surfaceView.setOnTouchListener(new View.OnTouchListener() {
            float prevX = 0;
            float prevY = 0;

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                float x = event.getX();
                float y = event.getY();

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        prevX = x;
                        prevY = y;
                        break;

                    case MotionEvent.ACTION_MOVE:
                        float dx = x - prevX;
                        float dy = y - prevY;

                        Log.d("JJH", "touch delta x : " + dx + " delta y : " + dy);
                        // 화면 해상도 비례로 정규화된 delta 회전값 전달
                        nativeOnTouchDelta(dx, dy);

                        prevX = x;
                        prevY = y;
                        break;
                }
                return true; // 이벤트 소비
            }
        });

        binding.buttonSelectImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_PICK);
                intent.setType("image/*");
                startActivityForResult(intent, REQUEST_PICK_IMAGE);
            }
        });

    }

    private final Choreographer.FrameCallback frameCallback = new Choreographer.FrameCallback() {
        @Override
        public void doFrame(long frameTimeNanos) {
            nativeRender();
            Choreographer.getInstance().postFrameCallback(this); // 다음 프레임에도 다시 호출
        }
    };

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == REQUEST_PICK_IMAGE && resultCode == RESULT_OK) {
            Uri imageUri = data.getData();
            try {
                Bitmap bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), imageUri);
                nativeSetImage(bitmap);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    /**
     * A native method that is implemented by the 'jihoon_mengine' native library,
     * which is packaged with this application.
     */
    private native boolean nativeInit(Surface surface);
    private native void nativeRelease();
    private native void nativeRender();
    private native void nativeOnSurfaceChanged(int width, int height);

    private native void nativeOnTouchDelta(float dx, float dy);

    private native void nativeSetImage(Bitmap bitmap);
}