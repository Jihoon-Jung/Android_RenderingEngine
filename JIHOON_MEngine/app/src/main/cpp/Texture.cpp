//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Texture.h"

void Texture::setTextureData(int width, int height, void* data)
{
    LOGI("Texture -> setTextureData");
    _imageData.assign((unsigned char*)data, (unsigned char*)data + width * height * 4);
    _width = width;
    _height = height;
    _hasImage = true;
    createTexture();
}

void Texture::createTexture() {
    if (_textureId == 0) {
        LOGI("Texture::createTexture - 텍스처 생성 시작");
        glGenTextures(1, &_textureId);
        CHECK_GL_ERROR();
        LOGI("Texture::createTexture - glGenTextures 호출 후 textureId: %u", _textureId);

        glBindTexture(GL_TEXTURE_2D, _textureId);
        CHECK_GL_ERROR();
        LOGI("Texture::createTexture - glBindTexture 호출");

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        CHECK_GL_ERROR();
        LOGI("Texture::createTexture - glTexImage2D 호출 전 - 크기: %dx%d", _width, _height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, _imageData.data());

        CHECK_GL_ERROR();
    } else {
        LOGI("Texture::createTexture - 이미 텍스처가 존재함 (textureId: %u)", _textureId);
    }
}

Texture::~Texture()
{
    LOGI("Remove Texture");
    glDeleteTextures(1, &_textureId);
    _textureId = 0;
}