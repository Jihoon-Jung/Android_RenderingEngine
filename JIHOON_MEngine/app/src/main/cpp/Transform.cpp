//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Transform.h"

void Transform::RotateByEulerDelta(float pitchDeg, float yawDeg) {
    float pitchRad = pitchDeg * M_PI / 180.0f;
    float yawRad   = yawDeg   * M_PI / 180.0f;

    // pitch = X축 회전, yaw = Y축 회전 (항공기 기준)
    Eigen::Quaternionf deltaRot =
            Eigen::AngleAxisf(pitchRad, Eigen::Vector3f::UnitX()) *
            Eigen::AngleAxisf(yawRad,   Eigen::Vector3f::UnitY());

    _rotation =  deltaRot * _rotation;  // 누적 회전
    UpdateModelMatrix();
}

void Transform::UpdateModelMatrix() {
    Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f R = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f S = Eigen::Matrix4f::Identity();

    // Translation
    T.block<3,1>(0,3) = _position;
    // 참고로 위 수식은
    // 3행 1열짜리 블록을, (0행, 3열)부터 추출한다 라는 의미다.
    // 즉 1x3 만큼 추출할건데 (0, 3)부터 추출하겠다라는 의미.
    // 그 후 T(4x4)의 같은 위치에 넣기.

    // Rotation (Quaternion → 3x3 회전행렬 → 4x4로 확장)
    R.block<3,3>(0,0) = _rotation.toRotationMatrix();

    // Scale
    S(0,0) = _scale.x();
    S(1,1) = _scale.y();
    S(2,2) = _scale.z();

    // 최종 모델 행렬 (T * R * S)
    _model = T * R * S;


    // Decompose position
    _position = _model.block<3, 1>(0, 3);
}

void Transform::RotateYawPitchLocal(float pitchDeg, float yawDeg) {
    const float pitchRad = pitchDeg * M_PI / 180.f;
    const float yawRad   = yawDeg   * M_PI / 180.f;

    //   주의: post-multiply → 로컬(Yaw→Pitch)  (FPS 스타일)
    Eigen::Quaternionf delta =
            Eigen::AngleAxisf(yawRad,   Eigen::Vector3f::UnitY()) *
            Eigen::AngleAxisf(pitchRad, Eigen::Vector3f::UnitX());

    _rotation = _rotation * delta;          // ← 기존 world 회전과 반대 순서
    UpdateModelMatrix();
}

