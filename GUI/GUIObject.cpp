#include "GUIObject.h"


GUIObject::GUIObject()
    : _position(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f), _rotation(0.0f),
    _size(0.0f, 0.0f),
    _isActive(true)
{
    _verticesTransformMatrixIs = false;
    _texCoordTransformMatrixIs = false;


    _vbo = OGLDriver::getInstance().CreateVBO(12800);

    GUIVertex vertices[4];
    /*vertices[0].position = glm::vec3(1.0f, 1.0f, 0.0f);
    vertices[1].position = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[2].position = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[3].position = glm::vec3(1.0f, 0.0f, 0.0f);*/
    vertices[0].position = glm::vec3(1.0f, 0.0f, 0.0f);
    vertices[1].position = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[2].position = glm::vec3(0.0f, -1.0f, 0.0f);
    vertices[3].position = glm::vec3(1.0f, -1.0f, 0.0f);

    vertices[0].texCoord = glm::vec2(0.0f, 0.0f);
    vertices[1].texCoord = glm::vec2(0.0f, 1.0f);
    vertices[2].texCoord = glm::vec2(1.0f, 1.0f);
    vertices[3].texCoord = glm::vec2(1.0f, 0.0f);

    _vbo->AddVertexData(vertices, 4);
}


GUIObject::~GUIObject()
{

}


void GUIObject::setSize(glm::vec2 size)
{
    _size = size;

    _verticesTransformMatrixIs = false;
    _texCoordTransformMatrixIs = false;
}


void GUIObject::calculateVerticesTransformMatrix()
{
    _verticesTransformMatrix = glm::translate(glm::vec3(_position.x, -_position.y, _position.z)) *
                               glm::rotate(_rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                               glm::scale(glm::vec3(_size, 1.0f)) * glm::scale(glm::vec3(_scale, 1.0f));
}


void GUIObject::calculateTexCoordTransformMatrix()
{
    _texCoordTransformMatrix = glm::mat4(1.0f);//glm::scale(glm::vec3(1.0f, -1.0f, 1.0f));//
}


void GUIObject::setPosition(glm::vec3 position)
{
    _position = position;

    _verticesTransformMatrixIs = false;
}


void GUIObject::setPosition(glm::vec2 position)
{
    setPosition(glm::vec3(position, 0.0f));
}


void GUIObject::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x, y, z));
}


void GUIObject::setScale(glm::vec2 scale)
{
    _scale = scale;

    _verticesTransformMatrixIs = false;
}


void GUIObject::setScale(float x, float y)
{
    setScale(glm::vec2(x, y));
}


void GUIObject::setRotation(float angle)
{
    _rotation = angle;

    _verticesTransformMatrixIs = false;
}



void GUIObject::move(glm::vec3 delta)
{
    _position += delta;

    _verticesTransformMatrixIs = false;
}


void GUIObject::move(glm::vec2 delta)
{
    move(glm::vec3(delta, 0.0f));
}


void GUIObject::move(float x, float y, float z)
{
    move(glm::vec3(x, y, z));
}


void GUIObject::scale(glm::vec2 delta)
{
    _scale *= delta;

    _verticesTransformMatrixIs = false;
}


void GUIObject::scale(float x, float y)
{
    scale(glm::vec2(x, y));
}


void GUIObject::rotate(float angle)
{
    _rotation += angle;

    _verticesTransformMatrixIs = false;
}


glm::vec3 GUIObject::getPosition()
{
    return _position;
}


glm::vec2 GUIObject::getScale()
{
    return _scale;
}


float GUIObject::getRotation()
{
    return _rotation;
}


glm::vec2 GUIObject::getSize()
{
    return _size;
}


void GUIObject::setIsActive(bool isActive)
{
    _isActive = isActive;
}


bool GUIObject::isActive()
{
    return _isActive;
}


VBO* GUIObject::getVBO()
{
    return _vbo;
}


glm::mat4& GUIObject::getVerticesTransformMatrix()
{
    if (!_verticesTransformMatrixIs)
    {
        calculateVerticesTransformMatrix();

        _verticesTransformMatrixIs = true;
    }

    return _verticesTransformMatrix;
}


glm::mat4& GUIObject::getTexCoordTransformMatrix()
{
    if (!_texCoordTransformMatrixIs)
    {
        calculateTexCoordTransformMatrix();

        _texCoordTransformMatrixIs = true;
    }

    return _texCoordTransformMatrix;
}
