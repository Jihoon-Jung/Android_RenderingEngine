//
// Created by jihoon jung on 2025. 6. 9..
//

#ifndef JIHOON_MENGINE_LIGHT_H
#define JIHOON_MENGINE_LIGHT_H
#include "Interface.h"
#include "Transform.h"
#include "Geometry.h"

class Light {
public:
    Light();
    ~Light(){}

    void Update();
    const Eigen::Matrix4f& getLightViewMat() const { return _lightViewMat; }
    const Eigen::Matrix4f& getLightProjMat() const { return _lightProjMat; }
    shared_ptr<Transform>& getTransform() { return _transform; }

    shared_ptr<Transform>& GetTransform() { return _transform; }
    shared_ptr<Geometry>& GetGeometry() { return _geometry; }

private:
    Eigen::Matrix4f _lightViewMat;
    Eigen::Matrix4f _lightProjMat;

    shared_ptr<Transform> _transform;
    shared_ptr<Geometry> _geometry;
};


#endif //JIHOON_MENGINE_LIGHT_H
