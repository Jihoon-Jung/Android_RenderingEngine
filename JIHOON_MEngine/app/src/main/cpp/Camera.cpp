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
    Eigen::Vector3f eye(_transform->getPosition() /*0.0f, -3.0f, 7.0f*/);
    Eigen::Vector3f at(0.0f, 0.0f, 0.0f);
    Eigen::Vector3f up(0.0f, 1.0f, 0.0f);

    Eigen::Vector3f zaxis = (eye - at).normalized();           // forward
    Eigen::Vector3f xaxis = up.cross(zaxis).normalized();          // right
    Eigen::Vector3f yaxis = zaxis.cross(xaxis);                    // true up

    _view <<  xaxis.x(), yaxis.x(), zaxis.x(), -xaxis.dot(eye),
            xaxis.y(), yaxis.y(), zaxis.y(), -yaxis.dot(eye),
            xaxis.z(), yaxis.z(), zaxis.z(), -zaxis.dot(eye),
            0,         0,         0,         1;



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