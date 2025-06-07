//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_RENDEROBJECT_H
#define JIHOON_MENGINE_RENDEROBJECT_H

#include "Interface.h"
#include "Transform.h"
#include "Geometry.h"
#include "Texture.h"

class RenderObject {
public:
    RenderObject();
    ~RenderObject();

    void Update();
    void rotateByEulerDelta(float pitch, float yaw);

    void setTextureData(int width, int height, void* data);
    void setMesh(string meshType);

    shared_ptr<Transform>& GetTransform() { return _transform; }
    shared_ptr<Geometry>& GetGeometry() { return _geometry; }
    shared_ptr<Texture>& GetTexture() { return _texture; }

private:
    shared_ptr<Transform> _transform;
    shared_ptr<Geometry> _geometry;
    shared_ptr<Texture> _texture;
};


#endif //JIHOON_MENGINE_RENDEROBJECT_H
