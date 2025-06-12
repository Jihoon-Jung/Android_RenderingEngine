//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_TRANSFORM_H
#define JIHOON_MENGINE_TRANSFORM_H

#include "Interface.h"
#include "SimpleMath.h"

class Transform {
public:
    Transform() {};
    ~Transform() {};

    void Update();
    void SetPosition(const Eigen::Vector3f& pos) { _position = pos; UpdateModelMatrix(); }
    void RotateByEulerDelta(float pitchDeg, float yawDeg);
    void SetScale(const Eigen::Vector3f& s) { _scale = s; UpdateModelMatrix(); }
    const Eigen::Matrix4f& GetModelMatrix() const { return _model; }

    const Eigen::Vector3f& getPosition() const { return _position; }
    const Eigen::Vector3f& getScale() const { return _scale; }

    Eigen::Vector3f getLook()  const { return -_model.block<3,1>(0, 2).normalized(); }  // ★반환 값
    Eigen::Vector3f getUp()    const { return _model.block<3,1>(0, 1).normalized(); }
    Eigen::Vector3f getRight() const { return _model.block<3,1>(0, 0).normalized(); }

    void RotateYawPitchLocal(float pitchDeg, float yawDeg);  // 로컬축 기준

private:

    Eigen::Vector3f _position = Eigen::Vector3f::Zero();
    Eigen::Quaternionf _rotation = Eigen::Quaternionf::Identity(); // 내부는 쿼터니언
    Eigen::Vector3f _scale = Eigen::Vector3f::Ones();

    Eigen::Matrix4f _model = Eigen::Matrix4f::Identity(); // 매 프레임 갱신
    float _accumulatedPitch = 0.0f;  // 누적된 pitch 회전값
    float _accumulatedYaw = 0.0f;    // 누적된 yaw 회전값
    void UpdateModelMatrix();
};




#endif //JIHOON_MENGINE_TRANSFORM_H
