package com.example.jihoon_mengine;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.SurfaceTexture;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.Choreographer;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.widget.*;

import androidx.appcompat.app.AppCompatActivity;

import com.example.jihoon_mengine.databinding.ActivityMainBinding;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("jihoon_mengine");
    }

    private ActivityMainBinding binding;

    private static final int REQUEST_PICK_IMAGE = 1001;
    private static final int REQUEST_PICK_TEXTURE = 2001;

    // Scene Edit 관련 변수
    private LinearLayout layoutMain;
    private View layoutSceneEdit;
    private Button buttonSelectMesh, buttonSelectTexture, buttonOk, buttonCancel;
    private String selectedMesh = null;
    private Bitmap selectedTexture = null;

    private final String[] meshList = {"Sphere", "Cube", "Pyramid"};

    private TextureView textureView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // TextureView 연결
        textureView = findViewById(R.id.textureView);
        textureView.setSurfaceTextureListener(new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
                Surface s = new Surface(surface);
                if (nativeInit(s))
                    Choreographer.getInstance().postFrameCallback(frameCallback);
                else
                    Log.e("JJH", "EGL Initialize Error");
            }

            @Override
            public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
                nativeOnSurfaceChanged(width, height);
            }

            @Override
            public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
                nativeRelease();
                return true;
            }

            @Override
            public void onSurfaceTextureUpdated(SurfaceTexture surface) {}
        });

        textureView.setOnTouchListener(new View.OnTouchListener() {
            float prevX = 0, prevY = 0;
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                float x = event.getX();
                float y = event.getY();
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        prevX = x; prevY = y; break;
                    case MotionEvent.ACTION_MOVE:
                        float dx = x - prevX, dy = y - prevY;
                        nativeOnTouchDelta(dx, dy);
                        prevX = x; prevY = y; break;
                }
                return true;
            }
        });

        // View 연결
        layoutMain = findViewById(R.id.layoutMain);
        layoutSceneEdit = findViewById(R.id.layoutSceneEdit);

        // "Make_Scene" 버튼
        findViewById(R.id.buttonMakeScene).setOnClickListener(v -> {
            nativeResetScene();
            selectedMesh = null;
            selectedTexture = null;
            buttonSelectMesh.setText("메쉬 선택");
            checkOkButton();
            layoutMain.setVisibility(View.GONE);
            layoutSceneEdit.setVisibility(View.VISIBLE);
        });

        // Scene 생성 화면 UI
        buttonSelectMesh = findViewById(R.id.buttonSelectMesh);
        buttonSelectMesh.setOnClickListener(v -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("메쉬 선택");
            builder.setItems(meshList, (dialog, which) -> {
                selectedMesh = meshList[which];
                buttonSelectMesh.setText("메쉬: " + selectedMesh);
                nativeSetMeshType(selectedMesh);
                Log.d("JJH", "call nativeSetMeshType -> meshType : " + selectedMesh);
                checkOkButton();
            });
            builder.show();
        });

        buttonSelectTexture = findViewById(R.id.buttonSelectTexture);
        buttonSelectTexture.setOnClickListener(v -> {
            Intent intent = new Intent(Intent.ACTION_PICK);
            intent.setType("image/*");
            startActivityForResult(intent, REQUEST_PICK_TEXTURE);
        });

        buttonOk = findViewById(R.id.buttonOk);
        buttonOk.setOnClickListener(v -> {
            layoutSceneEdit.setVisibility(View.GONE);
            layoutMain.setVisibility(View.VISIBLE);
        });

        buttonCancel = findViewById(R.id.buttonCancel);
        buttonCancel.setOnClickListener(v -> {
            layoutSceneEdit.setVisibility(View.GONE);
            layoutMain.setVisibility(View.VISIBLE);
        });
    }

    private void checkOkButton() {
        if (buttonOk != null)
            buttonOk.setEnabled(selectedMesh != null && selectedTexture != null);
    }

    private final Choreographer.FrameCallback frameCallback = new Choreographer.FrameCallback() {
        @Override
        public void doFrame(long frameTimeNanos) {
            nativeRender();
            Choreographer.getInstance().postFrameCallback(this);
        }
    };

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if ((requestCode == REQUEST_PICK_IMAGE || requestCode == REQUEST_PICK_TEXTURE)
                && resultCode == RESULT_OK) {
            Uri imageUri = data.getData();
            try {
                Bitmap bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), imageUri);

                // Scene 생성 UI에서의 텍스처 선택인 경우
                if (requestCode == REQUEST_PICK_TEXTURE) {
                    selectedTexture = bitmap;
                    checkOkButton();
                }

                // 모든 경우에 nativeSetImage 호출
                nativeSetImage(bitmap);
                Log.d("JJH", "call nativeSetImage");

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    // JNI 함수 선언
    private native boolean nativeInit(Surface surface);
    private native void nativeRelease();
    private native void nativeRender();
    private native void nativeOnSurfaceChanged(int width, int height);
    private native void nativeOnTouchDelta(float dx, float dy);
    private native void nativeSetImage(Bitmap bitmap);
    private native void nativeSetMeshType(String meshType);
    private native void nativeResetScene();
}