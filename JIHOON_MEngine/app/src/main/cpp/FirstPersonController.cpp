//
// Created by jihoon jung on 2025. 6. 12..
//

#include "FirstPersonController.h"
FirstPersonController::FirstPersonController(std::shared_ptr<Camera> cam)
        : _cam(std::move(cam)), _tr(_cam->getTransform())
{}

void FirstPersonController::AddViewDelta(float dx, float dy)
{
    _yawAcc   += dx * _mouseSens;
    _pitchAcc += dy * _mouseSens;
    _pitchAcc  = _clamp(_pitchAcc, -89.f, 89.f);      // 고개 꺾임 제한

    _tr->RotateYawPitchLocal(-dy * _mouseSens, dx * _mouseSens); // Pitch, Yaw (deg)
}

void FirstPersonController::Update(float dt)
{
    Eigen::Vector3f dir = Eigen::Vector3f::Zero();
    // 아날로그 스틱 방향 → 카메라 로컬축으로 변환
    dir += _tr->getRight() * _joyX;     // 좌/우
    dir += _tr->getLook()  * _joyY;     // 앞/뒤
    dir[1] = 0.f;                       // 점프·낙하 없으면 Y 고정

    if (dir.squaredNorm() > 0.f)
    {
        dir.normalize();
        _tr->SetPosition(_tr->getPosition() + dir * _moveSpeed * dt);
    }

    _cam->Update();   // 뷰·투영 행렬 동기화
}