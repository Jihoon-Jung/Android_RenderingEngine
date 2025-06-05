//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_GEOMETRY_H
#define JIHOON_MENGINE_GEOMETRY_H

#include "Interface.h"
struct Vertex
{
    Eigen::Vector3f position;
    Eigen::Vector2f uv;
};


class Geometry {
public:
    Geometry() {};
    ~Geometry() {};

    void CreateCube_Geometry();

    unsigned int GetVBO();
    unsigned int GetVAO();
    unsigned int GetEBO();

private:
    unsigned int VBO, VAO, EBO;
};



#endif //JIHOON_MENGINE_GEOMETRY_H
