//
// Created by jihoon jung on 2025. 6. 6..
//

#ifndef JIHOON_MENGINE_SCENE_H
#define JIHOON_MENGINE_SCENE_H
#include "RenderObject.h"

class Scene {
public:
    Scene();
    ~Scene();

    void setGeometryToRenderObject(string meshType);
    void setTextureToRenderObject(int width, int height, void *pixelData);
    shared_ptr<RenderObject>& getRenderObject() { return _renderObject; }
    shared_ptr<RenderObject>& getDefaultPlane() { return _defaultPlane; }
private:
    shared_ptr<RenderObject> _renderObject;
    shared_ptr<RenderObject> _defaultPlane;
};


#endif //JIHOON_MENGINE_SCENE_H
