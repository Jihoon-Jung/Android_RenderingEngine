package com.example.jihoon_mengine;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
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
import java.io.InputStream;

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
    private Bitmap assetBitmap = null;
    private final String[] meshList = {"Sphere", "Cube", "Pyramid"};

    private TextureView textureView;
    private JoystickView joystickView;


    private float prevX1 = 0, prevY1 = 0;  // 첫 번째 손가락
    private float prevX2 = 0, prevY2 = 0;  // 두 번째 손가락
    private boolean isTwoFingerTouch = false;
    private float twoFingerStartX = 0, twoFingerStartY = 0;  // 두 손가락 터치 시작점
    private boolean isFirstMove = true;  // 두 손가락 터치 후 첫 움직임인지 확인
    private boolean isHorizontalMove = false;  // true면 수평 이동, false면 수직 이동

    private boolean isToggleOn = false;

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
                if (nativeInit(s)) {
                    Choreographer.getInstance().postFrameCallback(frameCallback);
                    loadTextureFromAssets("wood.jpg");
                }
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

        // 조이스틱 설정
        joystickView = findViewById(R.id.joystickView);
        joystickView.setOnJoystickMovedListener((x, y) -> {
            // 조이스틱 이벤트를 네이티브 코드로 전달
            nativeOnJoystickMoved(x, y);
        });

        textureView.setOnTouchListener(new View.OnTouchListener() {
            float prevX = 0, prevY = 0;
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getActionMasked()) {
                    case MotionEvent.ACTION_DOWN:
                        // 첫 번째 손가락 터치
                        prevX1 = event.getX();
                        prevY1 = event.getY();
                        isTwoFingerTouch = false;
                        isFirstMove = true;  // 터치 시작할 때 초기화
                        break;

                    case MotionEvent.ACTION_POINTER_DOWN:
                        // 두 번째 손가락 터치
                        if (event.getPointerCount() == 2) {
                            isTwoFingerTouch = true;
                            isFirstMove = true;  // 두 손가락 터치 시작할 때 초기화
                            prevX1 = event.getX(0);
                            prevY1 = event.getY(0);
                            prevX2 = event.getX(1);
                            prevY2 = event.getY(1);
                            twoFingerStartX = (prevX1 + prevX2) / 2;
                            twoFingerStartY = (prevY1 + prevY2) / 2;
                        }
                        break;

                    case MotionEvent.ACTION_MOVE:
                        if (isTwoFingerTouch && event.getPointerCount() == 2) {
                            float currentX = (event.getX(0) + event.getX(1)) / 2;
                            float currentY = (event.getY(0) + event.getY(1)) / 2;

                            float dx = currentX - twoFingerStartX;
                            float dy = currentY - twoFingerStartY;

                            if (isFirstMove) {
                                // 첫 움직임일 때만 방향 결정
                                isHorizontalMove = Math.abs(dx) > Math.abs(dy);
                                isFirstMove = false;
                            }

                            // 결정된 방향으로만 회전
                            if (isHorizontalMove) {
                                nativeOnTwoFingerTouchDelta(dx, 0);
                            } else {
                                nativeOnTwoFingerTouchDelta(0, dy);
                            }

                            twoFingerStartX = currentX;
                            twoFingerStartY = currentY;
                        } else if (!isTwoFingerTouch) {
                            // 기존의 단일 터치 회전 처리
                            float x = event.getX();
                            float y = event.getY();
                            float dx = x - prevX1;
                            float dy = y - prevY1;
                            nativeOnTouchDelta(dx, dy);
                            prevX1 = x;
                            prevY1 = y;
                        }
                        break;

                    case MotionEvent.ACTION_UP:
                    case MotionEvent.ACTION_POINTER_UP:
                        if (event.getPointerCount() <= 1) {
                            isTwoFingerTouch = false;
                            isFirstMove = true;  // 터치가 끝날 때 초기화
                        }
                        break;
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
            nativeSetAssetImage(assetBitmap); // JNI로 Bitmap 전달
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

        Button buttonToggle = findViewById(R.id.buttonToggle);
        buttonToggle.setOnClickListener(v -> {
            isToggleOn = !isToggleOn;  // 상태 토글
            nativeOnToggleChanged(isToggleOn);  // C++에 상태 전달
            buttonToggle.setText(isToggleOn ? "ON" : "OFF");  // 버튼 텍스트 업데이트
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

    public void loadTextureFromAssets(String assetName) {
        try {
            AssetManager assetManager = getAssets();
            InputStream is = assetManager.open(assetName);
            Bitmap bitmap = BitmapFactory.decodeStream(is);
            is.close();
            assetBitmap = bitmap;
        } catch (IOException e) {
            e.printStackTrace();
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
    private native void nativeSetAssetImage(Bitmap bitmap);
    private native void nativeOnJoystickMoved(float x, float y);
    private native void nativeOnTwoFingerTouchDelta(float dx, float dy);
    private native void nativeOnToggleChanged(boolean isOn);
}