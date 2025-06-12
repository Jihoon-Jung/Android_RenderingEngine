package com.example.jihoon_mengine;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import androidx.annotation.Nullable;

public class JoystickView extends View {
    private float centerX = 0f;
    private float centerY = 0f;
    private float outerRadius = 0f;
    private float innerRadius = 0f;
    private float innerX = 0f;
    private float innerY = 0f;
    private boolean isDragging = false;

    private float joystickX = 0f;
    private float joystickY = 0f;

    private Paint paint;

    public interface OnJoystickMovedListener {
        void onJoystickMoved(float x, float y);
    }

    private OnJoystickMovedListener listener;

    public JoystickView(Context context) {
        super(context);
        init();
    }

    public JoystickView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        paint.setStyle(Paint.Style.FILL);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        centerX = w / 2f;
        centerY = h / 2f;
        outerRadius = Math.min(w, h) / 3f;
        innerRadius = outerRadius / 2f;
        innerX = centerX;
        innerY = centerY;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        // 외부 원
        paint.setColor(Color.argb(100, 128, 128, 128));
        canvas.drawCircle(centerX, centerY, outerRadius, paint);

        // 내부 원
        paint.setColor(Color.argb(200, 64, 64, 64));
        canvas.drawCircle(innerX, innerY, innerRadius, paint);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                if (isInJoystickArea(event.getX(), event.getY())) {
                    isDragging = true;
                    updateJoystickPosition(event.getX(), event.getY());
                }
                break;

            case MotionEvent.ACTION_MOVE:
                if (isDragging) {
                    updateJoystickPosition(event.getX(), event.getY());
                }
                break;

            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_CANCEL:
                isDragging = false;
                resetJoystick();
                break;
        }
        return true;
    }

    private void updateJoystickPosition(float x, float y) {
        float dx = x - centerX;
        float dy = y - centerY;
        float distance = (float) Math.sqrt(dx * dx + dy * dy);

        if (distance > outerRadius) {
            float angle = (float) Math.atan2(dy, dx);
            innerX = centerX + (float) Math.cos(angle) * outerRadius;
            innerY = centerY + (float) Math.sin(angle) * outerRadius;
        } else {
            innerX = x;
            innerY = y;
        }

        // 조이스틱 값을 -1.0 ~ 1.0 범위로 정규화
        joystickX = (innerX - centerX) / outerRadius;
        joystickY = (innerY - centerY) / outerRadius;

        invalidate();
        if (listener != null) {
            listener.onJoystickMoved(joystickX, joystickY);
        }
    }

    private void resetJoystick() {
        innerX = centerX;
        innerY = centerY;
        joystickX = 0f;
        joystickY = 0f;
        invalidate();
        if (listener != null) {
            listener.onJoystickMoved(0f, 0f);
        }
    }

    private boolean isInJoystickArea(float x, float y) {
        float dx = x - centerX;
        float dy = y - centerY;
        return Math.sqrt(dx * dx + dy * dy) <= outerRadius;
    }

    public void setOnJoystickMovedListener(OnJoystickMovedListener listener) {
        this.listener = listener;
    }
}