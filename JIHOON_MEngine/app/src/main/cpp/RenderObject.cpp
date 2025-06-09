//
// Created by jihoon jung on 2025. 6. 5..
//

#include "RenderObject.h"
RenderObject::RenderObject() {
    _transform = make_shared<Transform>();
    _geometry = make_shared<Geometry>();
    _texture = make_shared<Texture>();

    _geometry->createCube_Geometry();
}

RenderObject::~RenderObject() {
    LOGI("Remove RenderObject");
    _transform.reset();
    _geometry.reset();
    _texture.reset();
}

void RenderObject::Update() {
    _transform->Update();

}

void RenderObject::setTextureData(int width, int height, void *data) {
    _texture->setTextureData(width, height, data);
}

void RenderObject::setMesh(std::string meshType) {

    if (meshType == "Sphere") {
        LOGI("RenderObject->meshType = Sphere");
        _geometry->createSphere_Geometry();
    } else if (meshType == "Cube") {
        LOGI("RenderObject->meshType = Cube");
        _geometry->createCube_Geometry();
    } else if (meshType == "Pyramid") {
        LOGI("RenderObject->meshType = Pyramid");
        _geometry->createPyramid_Geometry();
    } else  if (meshType == "DefaultPlane"){
        LOGI("RenderObject->meshType = DefaultPlane");
        _geometry->createDefaultPlane(20, 20);
    }
}
