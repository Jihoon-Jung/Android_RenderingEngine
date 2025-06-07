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
    ~Geometry();

    void createSphere_Geometry();
    void createCube_Geometry();
    void createPyramid_Geometry();
    void createDefaultPlane(int sizeX, int sizeZ);
    unsigned int getVBO();
    unsigned int getVAO();
    unsigned int getEBO();

    int getIndexCount() { return _indexCount; }
private:
    unsigned int VBO, VAO, EBO;
    int _indexCount = 0;
};



#endif //JIHOON_MENGINE_GEOMETRY_H
