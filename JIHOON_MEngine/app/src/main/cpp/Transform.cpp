//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Transform.h"

void Transform::RotateByEulerDelta(float pitchDeg, float yawDeg) {
    float pitchRad = glm::radians(pitchDeg);
    float yawRad   = glm::radians(yawDeg);

    // pitch: X축 회전, yaw: Y축 회전 (항공기 기준)
    glm::quat deltaRot = glm::angleAxis(pitchRad, glm::vec3(1, 0, 0)) *
                         glm::angleAxis(yawRad,   glm::vec3(0, 1, 0));

    _rotation = deltaRot * _rotation;  // 누적 회전 (local 기준이면 오른쪽 곱)
    UpdateModelMatrix();
}

void Transform::UpdateModelMatrix() {
    // Translation
    glm::mat4 T = glm::translate(glm::mat4(1.0f), _position);

    // Rotation
    glm::mat4 R = glm::mat4_cast(_rotation);

    // Scale
    glm::mat4 S = glm::scale(glm::mat4(1.0f), _scale);

    // 모델 행렬 구성: T * R * S
    _model = T * R * S;

    // Decompose: 위치 추출 (역전은 아니고 그냥 업데이트)
    _position = glm::vec3(_model[3]);
}

