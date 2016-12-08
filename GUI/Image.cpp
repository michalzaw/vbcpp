#include "Image.h"


Image::Image(RTexture* texture)
    : _texture(texture),
    _size(0.0f, 0.0f),
    _isInitialized(false)
{
    createVBO();

    setTextureRect(UintRect(glm::uvec2(0, 0), texture->getSize()));

    _isInitialized = true;

    //setSize(texSize);

    //calculateVerticesTransformMatrix();
}


Image::Image(RTexture* texture, const UintRect& textureRect)
    : _texture(texture),
    _size(0.0f, 0.0f),
    _isInitialized(false)
{
    createVBO();

    setTextureRect(textureRect);

    _isInitialized = true;
}


Image::~Image()
{
    for (std::vector<VBO*>::iterator i = _vbos.begin(); i != _vbos.end(); ++i)
    {
        OGLDriver::getInstance().deleteVBO(*i);
    }
}


void Image::createVBO()
{
    if (_isInitialized)
        return;

    VBO* vbo = OGLDriver::getInstance().createVBO(12800);

    GUIVertex vertices[4];
    /*vertices[0].position = glm::vec3(1.0f, 1.0f, 0.0f);
    vertices[1].position = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[2].position = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[3].position = glm::vec3(1.0f, 0.0f, 0.0f);*/
    /*vertices[0].position = glm::vec3(1.0f, 0.0f, 0.0f);
    vertices[1].position = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[2].position = glm::vec3(0.0f, -1.0f, 0.0f);
    vertices[3].position = glm::vec3(1.0f, -1.0f, 0.0f);*/
    vertices[0].position = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[1].position = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[2].position = glm::vec3(1.0f, 1.0f, 0.0f);
    vertices[3].position = glm::vec3(1.0f, 0.0f, 0.0f);

    /*vertices[0].texCoord = glm::vec2(0.0f, 0.0f);
    vertices[1].texCoord = glm::vec2(0.0f, 1.0f);
    vertices[2].texCoord = glm::vec2(1.0f, 1.0f);
    vertices[3].texCoord = glm::vec2(1.0f, 0.0f);*/
    vertices[0].texCoord = glm::vec2(0.0f, 1.0f);
    vertices[1].texCoord = glm::vec2(0.0f, 0.0f);
    vertices[2].texCoord = glm::vec2(1.0f, 1.0f);
    vertices[3].texCoord = glm::vec2(1.0f, 0.0f);

    vbo->addVertexData(vertices, 4);

    _vbos.push_back(vbo);
}


void Image::setSize(glm::vec2 size)
{
    _size = size;

    changedVerticesTransformMatrixParameters();
}


void Image::calculateVerticesTransformMatrix()
{
    _verticesTransformMatrix = glm::translate(glm::vec3(_position.x, _position.y, _position.z)) *
                               glm::rotate(_rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                               glm::scale(glm::vec3(_size, 1.0f)) * glm::scale(glm::vec3(_scale, 1.0f));
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

    changedTexCoordTransformMatrixParameters();
}


RTexture* Image::getTexture()
{
    return _texture;
}


UintRect& Image::getTextureRect()
{
    return _textureRect;
}


glm::vec2 Image::getSize()
{
    return _size;
}


glm::mat4& Image::getVerticesTransformMatrix(unsigned int vboIndex)
{
  //  if (vboIndex != 0)
//        return glm::mat4(1.0f);


    if (!_verticesTransformMatrixIs)
    {
        calculateVerticesTransformMatrix();

        _verticesTransformMatrixIs = true;
    }

    return _verticesTransformMatrix;
}


glm::mat4& Image::getTexCoordTransformMatrix(unsigned int vboIndex)
{
  //  if (vboIndex != 0)
//        return glm::mat4(1.0f);


    if (!_texCoordTransformMatrixIs)
    {
        calculateTexCoordTransformMatrix();

        _texCoordTransformMatrixIs = true;
    }

    return _texCoordTransformMatrix;
}


void Image::addDataToRenderList(GUIRenderList* renderList)
{
    renderList->push_back(GUIRenderListElement(_vbos[0], 0,
                                               getVerticesTransformMatrix(), getTexCoordTransformMatrix(),
                                               _color, getTexture(),
                                               GUI_IMAGE_SHADER));
}
