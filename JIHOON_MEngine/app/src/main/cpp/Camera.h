//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_CAMERA_H
#define JIHOON_MENGINE_CAMERA_H

#include "Interface.h"

class Camera {
public:
    Camera() {};
    ~Camera() {};

    void initialize(int width, int height);
    void Update();
    void setViewportSize(int width, int height);

    Eigen::Matrix4f GetProjectionViewMat() const { return _proj * _view; }

private:

    int _surfaceWidth;
    int _surfaceHeight;

    Eigen::Matrix4f _view = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f _proj = Eigen::Matrix4f::Identity();
};




#endif //JIHOON_MENGINE_CAMERA_H
