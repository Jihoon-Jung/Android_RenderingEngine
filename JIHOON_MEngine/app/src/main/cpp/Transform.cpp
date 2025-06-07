//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Transform.h"

void Transform::RotateByEulerDelta(float pitchDeg, float yawDeg) {
    float pitchRad = pitchDeg * M_PI / 180.0f;
    float yawRad   = yawDeg   * M_PI / 180.0f;

    // pitch = X축 회전, yaw = Y축 회전 (항공기 기준)
    Eigen::Quaternionf deltaRot =
            Eigen::AngleAxisf(yawRad,   Eigen::Vector3f::UnitY()) *
            Eigen::AngleAxisf(pitchRad, Eigen::Vector3f::UnitX());

    _rotation = deltaRot * _rotation;  // 누적 회전
}

void Transform::UpdateModelMatrix() {
    Eigen::Affine3f t = Eigen::Affine3f::Identity();
    t.translate(_position);
    t.rotate(_rotation);
    t.scale(_scale);

    _model = t.matrix();  // 최종 4x4 행렬
}

void Transform::Update() {
    UpdateModelMatrix();
}