//
// Created by jihoon jung on 2025. 6. 6..
//

#include "Scene.h"

Scene::Scene() {
    _renderObject = make_shared<RenderObject>();
    _defaultPlane = make_shared<RenderObject>();
    _defaultPlane->setMesh("DefaultPlane");
}

Scene::~Scene() {
    LOGI("Remove Scene");
    _renderObject.reset();
    _defaultPlane.reset();
}
void Scene::setTextureToRenderObject(int width, int height, void *pixelData) {
    LOGI("Scene -> setTextureToRenderObject");
    _renderObject->setTextureData(width, height, pixelData);
}

void Scene::setGeometryToRenderObject(std::string meshType) {
    _renderObject->setMesh(meshType);
}