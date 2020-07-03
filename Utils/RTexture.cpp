#include "RTexture.h"

#include "../Graphics/OGLDriver.h"

#include <iostream>


RTexture::RTexture(std::string path, TextureType type, TextureFormat internalFormat, glm::uvec2 size, bool fromFile, bool compressed)
    : Resource(RT_TEXTURE, path),
    _textureType(type),
    _internalFormat(internalFormat),
    _size(size),
	_compressed(compressed),
	_fromFile(fromFile),
    _isGenerateMipmap(false)
{
    _format = _internalFormat;
    if (_format == TF_RGBA_16F || _format == TF_RGBA_32F)
        _format = TF_RGBA;
    if (_format == TF_RGB_16F || _format == TF_RGB_32F)
        _format = TF_RGB;

}


RTexture::~RTexture()
{
    std::cout << "RTexture - Destruktor: " << _path << std::endl;
    glDeleteTextures(1, &_texID);
}


void RTexture::setFiltering(TextureFilterMode minFilter, TextureFilterMode magFilter)
{
    bind();

    if ((minFilter == TFM_TRILINEAR || magFilter == TFM_TRILINEAR) && !_isGenerateMipmap)
    {
        generateMipmap();
    }

    glTexParameteri(_textureType, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(_textureType, GL_TEXTURE_MAG_FILTER, magFilter);
}


void RTexture::setClampMode(TextureClampMode mode)
{
    bind();

    glTexParameteri(_textureType, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(_textureType, GL_TEXTURE_WRAP_T, mode);
    //glTexParameteri(_textureType, GL_TEXTURE_WRAP_R, mode);
}


void RTexture::setAnisotropyFiltering(bool isEnable, float anisotropy)
{
    bind();

	float anisotropySamples = OGLDriver::getInstance().getMaxAnisotropy();
	if (anisotropy < anisotropySamples && anisotropy > 0.0f)
		anisotropySamples = anisotropy;

    glTexParameterf(_textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, isEnable ? anisotropySamples : 1.0f);
}


void RTexture::setParameter(GLenum name, GLint value)
{
    bind();

    glTexParameteri(_textureType, name, value);
}


void RTexture::setParameter(GLenum name, GLfloat value)
{
    bind();

    glTexParameterf(_textureType, name, value);
}


glm::uvec2 RTexture::getSize()
{
    return _size;
}


TextureType RTexture::getTextureType()
{
    return _textureType;
}


TextureFormat RTexture::getInternalFormat()
{
	return _internalFormat;
}


bool RTexture::isCompressed()
{
	return _compressed;
}


bool RTexture::isLoadedFromFile()
{
	return _fromFile;
}


void RTexture::generateMipmap()
{
	bind();

	glGenerateMipmap(_textureType);

	_isGenerateMipmap = true;
}
