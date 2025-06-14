//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Camera.h"

void Camera::initialize(int width, int height) {
    LOGI("Camera initialize");
    _surfaceWidth = width;
    _surfaceHeight = height;
    _transform = make_shared<Transform>();
}

void Camera::Update() {
    // View: simple translation (camera back from origin)
    Eigen::Vector3f eye = _transform->getPosition();
    Eigen::Vector3f look = _transform->getLook();     // Z- 방향
    Eigen::Vector3f at = eye + look;
    Eigen::Vector3f up(0, 1, 0);//Eigen::Vector3f up = _transform->getUp();         // Y+ 방향
    /* 1. 월드 기준 Up 방향을 항상 유지
        예를 들어 게임에서 카메라는 항상 "지면이 수평"이어야 하고, Y축이 항상 위라고 가정

        사용자가 마우스를 좌우로 움직이면 **Yaw(수평 회전)**만 하고,
        상하로 움직이면 Pitch만 일어나며,
        롤(Roll) 은 허용하지 않음

        즉, look 벡터는 변해도, up 벡터는 항상 (0, 1, 0)으로 유지되어야 한다는 의도
    */
    // 뷰 행렬 생성 (OpenGL RH 기준)
    Eigen::Vector3f zaxis = (eye - at).normalized();
    Eigen::Vector3f xaxis = up.cross(zaxis).normalized();
    Eigen::Vector3f yaxis = zaxis.cross(xaxis);

    _view <<
          xaxis.x(), xaxis.y(), xaxis.z(), -xaxis.dot(eye),
            yaxis.x(), yaxis.y(), yaxis.z(), -yaxis.dot(eye),
            zaxis.x(), zaxis.y(), zaxis.z(), -zaxis.dot(eye),
            0,         0,         0,          1;


    // Projection: perspective
    float aspect = (float)_surfaceWidth / (float)_surfaceHeight;
    float fov = 45.0f * M_PI / 180.0f;
    float near = 0.1f, far = 100.0f;
    float f = 1.0f / tan(fov / 2);
    _proj << f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (far+near)/(near-far), (2*far*near)/(near-far),
            0, 0, -1, 0;

}


void Camera::setViewportSize(int width, int height) {
    LOGI("Camera surface view size is changed");
    _surfaceWidth = width;
    _surfaceHeight = height;
}