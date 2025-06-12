//
// Created by jihoon jung on 2025. 6. 12..
//

#ifndef JIHOON_MENGINE_FIRSTPERSONCONTROLLER_H
#define JIHOON_MENGINE_FIRSTPERSONCONTROLLER_H
#include "Camera.h"

class FirstPersonController {
public:
    explicit FirstPersonController(std::shared_ptr<Camera> cam);

    /* 마우스/터치 drag 한 프레임분 이동량 전달 (픽셀) */
    void AddViewDelta(float dx, float dy);

    /* 키 입력(WASD 등) 상태 전달 → 매 프레임 Update 전에 호출 */
    struct MoveState { bool forward, back, left, right; };
    void SetMoveState(const MoveState& s) { _move = s; }

    /* 프레임마다 호출 */
    void Update(float deltaTime);

    /* 파라미터 조정 */
    void SetMoveSpeed(float mps)       { _moveSpeed = mps; }
    void SetMouseSensitivity(float s)  { _mouseSens  = s; }
    void SetJoystick(float strafe, float forward) {      // x = 좌/우, y = 앞/뒤
        _joyX = strafe;   // -1 ←, +1 →
        _joyY = forward;  // +1 ↑(앞), -1 ↓(뒤)  (필요하면 방향 반전)
    }
private:
    std::shared_ptr<Camera>   _cam;
    std::shared_ptr<Transform> _tr;

    float _pitchAcc = 0.f;   // 누적 Pitch (deg)
    float _yawAcc   = 0.f;   // 누적 Yaw   (deg)

    float _mouseSens = 0.2f; // deg/pixel
    float _moveSpeed = 5.f;  // m/s
    float _joyX = 0.f, _joyY = 0.f;   // -1 ~ 1
    MoveState _move {false,false,false,false};

    static float _clamp(float v, float mn, float mx) { return std::max(mn, std::min(v, mx)); }
};


#endif //JIHOON_MENGINE_FIRSTPERSONCONTROLLER_H
