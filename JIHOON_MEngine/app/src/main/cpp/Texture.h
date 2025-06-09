//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_TEXTURE_H
#define JIHOON_MENGINE_TEXTURE_H
#include "Interface.h"

class Texture {
public:
    Texture() {};
    ~Texture();
    void setTextureData(int width, int height, void* data);
    void createTexture();
    unsigned int getTextureId() { return _textureId; }
private:
    std::vector<unsigned char> _imageData;
    int _width = 0;
    int _height = 0;
    unsigned int _textureId = 0;
};


#endif //JIHOON_MENGINE_TEXTURE_H
