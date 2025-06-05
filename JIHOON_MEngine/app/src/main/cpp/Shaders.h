//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_SHADERS_H
#define JIHOON_MENGINE_SHADERS_H

#include "Interface.h"

class Shaders {
public:
    Shaders();
    ~Shaders();
    bool CreateShaderProgram();
    unsigned int GetShaderProgram(string s);
private:
    map<string, int> shaderMap;

};
#endif //JIHOON_MENGINE_SHADERS_H
