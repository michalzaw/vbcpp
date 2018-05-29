#include "RTexture.h"

#include <iostream>


RTexture::RTexture(std::string path, TextureType type, TextureFormat format, glm::uvec2 size)
    : Resource(path),
    _type(type),
    _format(format),
    _size(size),
    _isGenerateMipmap(false)
{

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


void RTexture::setParameter(GLenum name, GLint value)
{
    bind();

    glTexParameteri(_type, name, value);
}


void RTexture::setParameter(GLenum name, GLfloat value)
{
    bind();

    glTexParameterf(_type, name, value);
}


glm::uvec2 RTexture::getSize()
{
    return _size;
}
