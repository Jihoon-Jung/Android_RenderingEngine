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
    void RotateByEulerDelta(float pitchDeg, float yawDeg);

    const glm::vec3& getPosition() const { return _position; }
    const glm::vec3& getScale()    const { return _scale; }
    const glm::quat& getRotation() const { return _rotation; }
    const glm::mat4& getModelMatrix() const { return _model; }

    void SetPosition(const glm::vec3& pos) { _position = pos; UpdateModelMatrix(); }
    void SetScale(const glm::vec3& scale)  { _scale = scale;  UpdateModelMatrix(); }
    void setRotation(const glm::quat& rot) { _rotation = rot; UpdateModelMatrix(); }

    glm::vec3 getForward() const { return _rotation * glm::vec3(0.0f, 0.0f, -1.0f); }
    glm::vec3 getRight()   const { return _rotation * glm::vec3(1.0f, 0.0f, 0.0f); }
    glm::vec3 getUp()      const { return _rotation * glm::vec3(0.0f, 1.0f, 0.0f); }

    void RotateYawPitchLocal(float pitchDeg, float yawDeg);  // 로컬축 기준

private:

    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _scale    = glm::vec3(1.0f);
    glm::quat _rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // identity
    glm::mat4 _model    = glm::mat4(1.0f);

    float _accumulatedPitch = 0.0f;  // 누적된 pitch 회전값
    float _accumulatedYaw = 0.0f;    // 누적된 yaw 회전값
    void UpdateModelMatrix();
};




#endif //JIHOON_MENGINE_TRANSFORM_H
