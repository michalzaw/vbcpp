#include "RTexture2D.h"

#include <iostream>

#include "Logger.h"


RTexture2D::RTexture2D(string path, unsigned char* data, TextureFormat internalFormat, glm::uvec2 size)
    : RTexture(path, TT_2D, internalFormat, size)
{
    std::cout << "RTexture2D - Konstruktor: " << _path << std::endl;


    glGenTextures(1, &_texID);
    bind();

    glTexImage2D(_textureType, 0, _internalFormat, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, data);
}


RTexture2D::RTexture2D(string path, float* data, TextureFormat internalFormat, glm::uvec2 size)
    : RTexture(path, TT_2D, internalFormat, size)
{
    std::cout << "RTexture2D - Konstruktor: " << _path << std::endl;


    glGenTextures(1, &_texID);
    bind();

    glTexImage2D(_textureType, 0, _internalFormat, _size.x, _size.y, 0, _format, GL_FLOAT, data);
}


RTexture2D::RTexture2D(TextureFormat internalFormat, glm::uvec2 size, bool isMultisample, int samplesCount)
    : RTexture("", isMultisample ? TT_2D_MULTISAMPLE : TT_2D, internalFormat, size)
{
    std::cout << "RTexture2D - Konstruktor: " << _path << std::endl;


    glGenTextures(1, &_texID);
    bind();

    if (_textureType == TT_2D_MULTISAMPLE)
        glTexImage2DMultisample(_textureType, samplesCount, _internalFormat, _size.x, _size.y, GL_TRUE);
    else
        glTexImage2D(_textureType, 0, _internalFormat, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, NULL);
}


RTexture2D::RTexture2D(string path, GLuint textureId, glm::uvec2 size, TextureFilterMode minFilter, TextureFilterMode magFilter)
	: RTexture(path, TT_2D, TF_RGBA, size)
{

	_texID = textureId;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}


RTexture2D::~RTexture2D()
{

}


void RTexture2D::setTexSubImage(unsigned char* data, int offsetX, int offsetY, int width, int height)
{
    if (offsetX + width > _size.x || offsetY + height > _size.y)
        return;

    bind();

    glTexSubImage2D(_textureType, 0, offsetX, offsetY, width, height, _format, GL_UNSIGNED_BYTE, data);

    _isGenerateMipmap = false;
}


void RTexture2D::setTexSubImage(float* data, int offsetX, int offsetY, int width, int height)
{
    if (offsetX + width > _size.x || offsetY + height > _size.y)
        return;

    bind();

    glTexSubImage2D(_textureType, 0, offsetX, offsetY, width, height, _format, GL_FLOAT, data);

    _isGenerateMipmap = false;
}


RTexture2D* RTexture2D::createWhiteTexture(string path, glm::uvec2 size)
{
    unsigned char data[16] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
    RTexture2D* texture = new RTexture2D(path, data, TF_RGBA, size);
    texture->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
    texture->setClampMode(TCM_REPEAT);

    return texture;
}
