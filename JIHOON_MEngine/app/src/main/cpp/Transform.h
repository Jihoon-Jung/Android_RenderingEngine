//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_TRANSFORM_H
#define JIHOON_MENGINE_TRANSFORM_H

#include "Interface.h"

class Transform {
public:
    Transform() {};
    ~Transform() {};

    void Update();
    void SetPosition(const Eigen::Vector3f& pos) { _position = pos; }
    void SetRotationEuler(const Eigen::Vector3f& eulerDeg); // 외부에서는 오일러로 설정
    void SetScale(const Eigen::Vector3f& s) { _scale = s; }

    const Eigen::Matrix4f& GetModelMatrix() const { return _model; }
private:

    Eigen::Vector3f _position = Eigen::Vector3f::Zero();
    Eigen::Quaternionf _rotation = Eigen::Quaternionf::Identity(); // 내부는 쿼터니언
    Eigen::Vector3f _scale = Eigen::Vector3f::Ones();

    Eigen::Matrix4f _model = Eigen::Matrix4f::Identity(); // 매 프레임 갱신

    void UpdateModelMatrix();
};




#endif //JIHOON_MENGINE_TRANSFORM_H
