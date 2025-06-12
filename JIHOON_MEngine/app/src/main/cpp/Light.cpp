//
// Created by jihoon jung on 2025. 6. 9..
//

#include "Light.h"

Light::Light() {
    _transform = make_shared<Transform>();
    _geometry = make_shared<Geometry>();
    _geometry->createCube_Geometry();
}

void Light::Update() {

    Eigen::Vector3f eye = _transform->getPosition();
    Eigen::Vector3f look = _transform->getLook();     // Z- 방향
    Eigen::Vector3f at = eye + look;
    Eigen::Vector3f up = _transform->getUp();         // Y+ 방향

    // 뷰 행렬 생성 (OpenGL RH 기준)
    Eigen::Vector3f zaxis = (eye - at).normalized();
    Eigen::Vector3f xaxis = up.cross(zaxis).normalized();
    Eigen::Vector3f yaxis = zaxis.cross(xaxis);

    _lightViewMat <<
          xaxis.x(), yaxis.x(), zaxis.x(), -xaxis.dot(eye),
            xaxis.y(), yaxis.y(), zaxis.y(), -yaxis.dot(eye),
            xaxis.z(), yaxis.z(), zaxis.z(), -zaxis.dot(eye),
            0,         0,         0,         1;


    const float l = -10.0f, r = 10.0f;
    const float b = -10.0f, t = 10.0f;
    const float n = 1.0f,  f = 20.0f;

    _lightProjMat.setZero();
    _lightProjMat(0,0) =  2.0f / (r - l);
    _lightProjMat(1,1) =  2.0f / (t - b);
    _lightProjMat(2,2) = -2.0f / (f - n);
    _lightProjMat(0,3) = -(r + l) / (r - l);
    _lightProjMat(1,3) = -(t + b) / (t - b);
    _lightProjMat(2,3) = -(f + n) / (f - n);
    _lightProjMat(3,3) =  1.0f;

//    // Projection: perspective
//    float aspect = (float)2960 / (float)1752;
//    float fov = 45.0f * M_PI / 180.0f;
//    float near = 0.1f, far = 100.0f;
//    float f = 1.0f / tan(fov / 2);
//    _lightProjMat << f / aspect, 0, 0, 0,
//            0, f, 0, 0,
//            0, 0, (far+near)/(near-far), (2*far*near)/(near-far),
//            0, 0, -1, 0;


}


