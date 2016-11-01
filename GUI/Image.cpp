#include "Image.h"


Image::Image(RTexture* texture)
    : _texture(texture)
{
    setTextureRect(UintRect(glm::uvec2(0, 0), texture->getSize()));

    //setSize(texSize);

    //calculateVerticesTransformMatrix();
}


Image::Image(RTexture* texture, const UintRect& textureRect)
    : _texture(texture),
    _textureRect(textureRect)
{
    /*_size = _textureRect.size;

    calculateVerticesTransformMatrix();
    calculateTexCoordTranformMatrix();*/

    setSize(_textureRect.size);
}


Image::~Image()
{

}


void Image::calculateTexCoordTransformMatrix()
{
    _texCoordTransformMatrix =
                               glm::translate(glm::vec3(_textureRect.position.x / (float)_texture->getSize().x, _textureRect.position.y / (float)_texture->getSize().y, 1.0f)) *
                               glm::scale(glm::vec3(_textureRect.size.x / (float)_texture->getSize().x, _textureRect.size.y / (float)_texture->getSize().y, 1.0f)) *
                               glm::scale(glm::vec3(1.0f, -1.0f, 1.0f));

}


void Image::setTexture(RTexture* texture, bool isResetRect)
{
    _texture = texture;

    if (isResetRect)
    {
        setTextureRect(UintRect(glm::uvec2(0, 0), texture->getSize()));
    }
}


void Image::setTextureRect(const UintRect& rect)
{
    _textureRect = rect;

    setSize(_textureRect.size);
}


RTexture* Image::getTexture()
{
    return _texture;
}


UintRect& Image::getTextureRect()
{
    return _textureRect;
}
