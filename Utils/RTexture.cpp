#include "RTexture.h"

#include <iostream>

/*RTexture::RTexture(string path, GLuint id, TextureType type, glm::uvec2 size)
    : Resource(path),
    _texID(id),
    _type(type),
    _size(size),
    _isGenerateMipmap(false);
{
    std::cout << "RTexture - Konstruktor: " << _path << std::endl;
}*/


RTexture::RTexture(string path, unsigned char* data, TextureType type, TextureFormat format, glm::uvec2 size)
    : Resource(path),
    _type(type),
    _format(format),
    _size(size),
    _isGenerateMipmap(false)
{
    std::cout << "RTexture - Konstruktor: " << _path << std::endl;


    glGenTextures(1, &_texID);
    bind();

    glTexImage2D(_type, 0, _format, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, data);
}


RTexture::RTexture(TextureType type, TextureFormat format, glm::uvec2 size)
    : Resource(""),
    _type(type),
    _format(format),
    _size(size),
    _isGenerateMipmap(false)
{
    std::cout << "RTexture - Konstruktor: " << _path << std::endl;


    glGenTextures(1, &_texID);
    bind();

    glTexImage2D(_type, 0, _format, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, NULL);
}


RTexture::~RTexture()
{
    std::cout << "RTexture - Destruktor: " << _path << std::endl;
    glDeleteTextures(1, &_texID);
}


void RTexture::generateMipmap()
{
    glGenerateMipmap(_type);

    _isGenerateMipmap = true;
}


void RTexture::setFiltering(TextureFilterMode minFilter, TextureFilterMode magFilter)
{
    bind();

    if ((minFilter == TFM_TRILINEAR || magFilter == TFM_TRILINEAR) && !_isGenerateMipmap)
    {
        generateMipmap();
    }

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


void RTexture::setTexSubImage(unsigned char* data, int offsetX, int offsetY, int width, int height)
{
    if (offsetX + width > _size.x || offsetY + height > _size.y)
        return;

    glTexSubImage2D(_type, 0, offsetX, offsetY, width, height, _format, GL_UNSIGNED_BYTE, data);
}


glm::uvec2 RTexture::getSize()
{
    return _size;
}
