//
// Created by jihoon jung on 2025. 6. 5..
//

#include "RenderObject.h"
RenderObject::RenderObject() {
    _transform = make_shared<Transform>();
    _geometry = make_shared<Geometry>();

    _geometry->CreateCube_Geometry();
}

void RenderObject::Update() {
    _transform->Update();

}
