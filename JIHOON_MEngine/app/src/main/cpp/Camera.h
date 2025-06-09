//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_CAMERA_H
#define JIHOON_MENGINE_CAMERA_H

#include "Interface.h"
#include "Transform.h"

class Camera {
public:
    Camera() {};
    ~Camera() {};

    void initialize(int width, int height);
    void Update();
    void setViewportSize(int width, int height);

    Eigen::Matrix4f GetProjectionViewMat() const { return _proj * _view; }
    Eigen::Matrix4f getViewMat() const { return _view; }
    Eigen::Matrix4f getProjMat() const { return _proj; }
    shared_ptr<Transform>& getTransform() { return _transform; }
private:

    int _surfaceWidth;
    int _surfaceHeight;

    Eigen::Matrix4f _view = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f _proj = Eigen::Matrix4f::Identity();

    shared_ptr<Transform> _transform;
};




#endif //JIHOON_MENGINE_CAMERA_H
