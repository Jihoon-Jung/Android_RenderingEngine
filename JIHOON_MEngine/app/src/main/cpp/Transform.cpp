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

    Eigen::Matrix3f rotMat = _rotation.toRotationMatrix();
    LOGE("Rotation Matrix:\n"
         "[%f, %f, %f]\n"
         "[%f, %f, %f]\n"
         "[%f, %f, %f]\n",
         rotMat(0, 0), rotMat(0, 1), rotMat(0, 2),
         rotMat(1, 0), rotMat(1, 1), rotMat(1, 2),
         rotMat(2, 0), rotMat(2, 1), rotMat(2, 2));
}

void Transform::UpdateModelMatrix() {
    Eigen::Affine3f t = Eigen::Affine3f::Identity();
    t.translate(_position);
    t.rotate(_rotation);
    t.scale(_scale);

    _model = t.matrix();  // 최종 4x4 행렬

    LOGE("Model Matrix:\n"
         "[%f, %f, %f, %f]\n"
         "[%f, %f, %f, %f]\n"
         "[%f, %f, %f, %f]\n"
         "[%f, %f, %f, %f]\n",
         _model(0, 0), _model(0, 1), _model(0, 2), _model(0, 3),
         _model(1, 0), _model(1, 1), _model(1, 2), _model(1, 3),
         _model(2, 0), _model(2, 1), _model(2, 2), _model(2, 3),
         _model(3, 0), _model(3, 1), _model(3, 2), _model(3, 3));
}

void Transform::Update() {
    UpdateModelMatrix();
}