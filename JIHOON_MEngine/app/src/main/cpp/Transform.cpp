//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Transform.h"

void Transform::SetRotationEuler(const Eigen::Vector3f& eulerDeg) {
    Eigen::Vector3f eulerRad = eulerDeg * (M_PI / 180.0f);
    // 오일러각 (roll, pitch, yaw) → 쿼터니언
    _rotation = Eigen::AngleAxisf(eulerRad.z(), Eigen::Vector3f::UnitZ()) *
                Eigen::AngleAxisf(eulerRad.y(), Eigen::Vector3f::UnitY()) *
                Eigen::AngleAxisf(eulerRad.x(), Eigen::Vector3f::UnitX());
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