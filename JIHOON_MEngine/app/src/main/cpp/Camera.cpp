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
    glm::vec3 eye = _transform->getPosition();
    glm::vec3 look = _transform->getForward();
    glm::vec3 at = eye + look;
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    _view = glm::lookAt(eye, at, up);

    float aspect = static_cast<float>(_surfaceWidth) / static_cast<float>(_surfaceHeight);
    float fovDeg = 45.0f;
    float near = 0.1f, far = 100.0f;
    _proj = glm::perspective(glm::radians(fovDeg), aspect, near, far);
}



void Camera::setViewportSize(int width, int height) {
    LOGI("Camera surface view size is changed");
    _surfaceWidth = width;
    _surfaceHeight = height;
}