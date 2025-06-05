//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_RENDEROBJECT_H
#define JIHOON_MENGINE_RENDEROBJECT_H

#include "Interface.h"
#include "Transform.h"
#include "Geometry.h"

class RenderObject {
public:
    RenderObject();
    ~RenderObject() {};

    void Update();
    void rotateByEulerDelta(float pitch, float yaw);

    shared_ptr<Transform>& GetTransform() { return _transform; }
    shared_ptr<Geometry>& GetGeometry() { return _geometry; }
private:
    shared_ptr<Transform> _transform;
    shared_ptr<Geometry> _geometry;
};


#endif //JIHOON_MENGINE_RENDEROBJECT_H
