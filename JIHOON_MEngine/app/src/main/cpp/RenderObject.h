//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_RENDEROBJECT_H
#define JIHOON_MENGINE_RENDEROBJECT_H

#include "Interface.h"

class RenderObject {
public:
    RenderObject();
    ~RenderObject() {};

    void Update();

private:
    shared_ptr<Transform> _transform;
    shared_ptr<Geometry> _geometry;
};


#endif //JIHOON_MENGINE_RENDEROBJECT_H
