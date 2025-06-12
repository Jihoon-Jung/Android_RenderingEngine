//
// Created by jihoon jung on 2025. 6. 12..
//

#ifndef JIHOON_MENGINE_SIMPLEMATH_H
#define JIHOON_MENGINE_SIMPLEMATH_H

#include "Interface.h"
namespace SimpleMath {
    using mat4 = Eigen::Matrix4f;
    using vec3 = Eigen::Vector3f;
    using vec2 = Eigen::Vector2f;

    // radians: degree -> radian 변환
    inline float radians(float deg) {
        return deg * float(M_PI) / 180.0f;
    }

    // normalize: 벡터 정규화
    inline vec3 normalize(const vec3& v) {
        return v.normalized();
    }

    // translate: 4x4 행렬에 평행이동 적용
    inline mat4 translate(const mat4& m, const vec3& v) {
        mat4 result = m;
        // column-major 기준 (OpenGL)
        result.block<3, 1>(0, 3) += result.block<3, 3>(0, 0) * v;
        return result;
    }

    inline mat4 scale(const mat4& m, const vec3& v) {
        mat4 scaleMat = mat4::Identity();
        scaleMat(0,0) = v.x();
        scaleMat(1,1) = v.y();
        scaleMat(2,2) = v.z();
        return m * scaleMat;
    }

    // rotate: 4x4 행렬에 회전 적용 (radians, 축 기준)
    inline mat4 rotate(const mat4& m, float angleRad, const vec3& axis) {
        Eigen::AngleAxisf rot(angleRad, axis.normalized());
        mat4 rotMat = mat4::Identity();
        rotMat.block<3, 3>(0, 0) = rot.toRotationMatrix();
        return m * rotMat;
    }

    // lookAt: 카메라 View 행렬 (OpenGL 기준)
    inline mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
        vec3 f = (center - eye).normalized();
        vec3 s = f.cross(up).normalized();
        vec3 u = s.cross(f);

        mat4 m = mat4::Identity();
        m(0,0) = s.x(); m(0,1) = u.x(); m(0,2) = -f.x();
        m(1,0) = s.y(); m(1,1) = u.y(); m(1,2) = -f.y();
        m(2,0) = s.z(); m(2,1) = u.z(); m(2,2) = -f.z();
        m(0,3) = -s.dot(eye);
        m(1,3) = -u.dot(eye);
        m(2,3) = f.dot(eye);
        return m;
    }

    // perspective: 원근 투영행렬 (fovy: radians, aspect: w/h)
    inline mat4 perspective(float fovy, float aspect, float zNear, float zFar) {
        float tanHalfFovy = tan(fovy / 2.0f);
        mat4 m = mat4::Zero();
        m(0,0) = 1.0f / (aspect * tanHalfFovy);
        m(1,1) = 1.0f / tanHalfFovy;
        m(2,2) = -(zFar + zNear) / (zFar - zNear);
        m(2,3) = -(2.0f * zFar * zNear) / (zFar - zNear);
        m(3,2) = -1.0f;
        return m;
    }

    // ortho: 직교 투영행렬 (OpenGL 방식)
    inline mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
        mat4 m = mat4::Identity();
        m(0,0) = 2.0f / (right - left);
        m(1,1) = 2.0f / (top - bottom);
        m(2,2) = -2.0f / (zFar - zNear);
        m(0,3) = -(right + left) / (right - left);
        m(1,3) = -(top + bottom) / (top - bottom);
        m(2,3) = -(zFar + zNear) / (zFar - zNear);
        return m;
    }
}
#endif //JIHOON_MENGINE_SIMPLEMATH_H
