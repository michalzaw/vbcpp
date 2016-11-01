#include "RTexture.h"

#include <iostream>

RTexture::RTexture(string path, GLuint id, TextureType type, glm::uvec2 size)
    : Resource(path),
    _texID(id),
    _type(type),
    _size(size)
{
    std::cout << "RTexture - Konstruktor: " << _path << std::endl;
}

RTexture::~RTexture()
{
    std::cout << "RTexture - Destruktor: " << _path << std::endl;
    glDeleteTextures(1, &_texID);
}


void RTexture::setFiltering(TextureFilterMode minFilter, TextureFilterMode magFilter)
{
    bind();

    glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, magFilter);
}


void RTexture::setClampMode(TextureClampMode mode)
{
    bind();

    glTexParameteri(_type, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(_type, GL_TEXTURE_WRAP_T, mode);
    glTexParameteri(_type, GL_TEXTURE_WRAP_R, mode);
}


void RTexture::setAnisotropyFiltering(bool isEnable, float anisotropy)
{
    bind();

    glTexParameterf(_type, GL_TEXTURE_MAX_ANISOTROPY_EXT, isEnable ? anisotropy : 1.0f);
}


glm::uvec2 RTexture::getSize()
{
    return _size;
}
