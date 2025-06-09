//
// Created by jihoon jung on 2025. 6. 9..
//

#ifndef JIHOON_MENGINE_LIGHT_H
#define JIHOON_MENGINE_LIGHT_H
#include "Interface.h"

class Light {
public:
    Light();
    ~Light(){}

    void createLightViewProjMat();

private:
    Eigen::Matrix4f _lightViewMat;
    Eigen::Matrix4f _lightProjMat;
};


#endif //JIHOON_MENGINE_LIGHT_H
