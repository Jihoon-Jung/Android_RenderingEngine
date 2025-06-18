#ifndef JIHOON_MENGINE_CAMERA_H
#define JIHOON_MENGINE_CAMERA_H

#include "Interface.h"
#include "Transform.h"
#include "SimpleMath.h"


class Camera {
public:
    Camera() {};
    ~Camera() {};

    void initialize(int width, int height);
    void Update();
    void setViewportSize(int width, int height);

    glm::mat4 GetProjectionViewMat() const { return _proj * _view; }
    const glm::mat4& getViewMat() const { return _view; }
    const glm::mat4& getProjMat() const { return _proj; }
    shared_ptr<Transform>& getTransform() { return _transform; }

private:
    int _surfaceWidth;
    int _surfaceHeight;

    glm::mat4 _view = glm::mat4(1.0f);
    glm::mat4 _proj = glm::mat4(1.0f);

    shared_ptr<Transform> _transform;
};

#endif //JIHOON_MENGINE_CAMERA_H
