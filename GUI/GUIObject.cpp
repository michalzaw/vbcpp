#include "GUIObject.h"


GUIObject::GUIObject()
    : _position(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f), _rotation(0.0f),
    _color(1.0f, 1.0f, 1.0f, 1.0f),
    _isActive(true),
    _identityTransformMatrix(1.0f)
{

}


GUIObject::~GUIObject()
{

}


void GUIObject::setPosition(glm::vec3 position)
{
    _position = position;

    changedVerticesTransformMatrixParameters();
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

    changedVerticesTransformMatrixParameters();
}


void GUIObject::setScale(float x, float y)
{
    setScale(glm::vec2(x, y));
}


void GUIObject::setRotation(float angle)
{
    _rotation = angle;

    changedVerticesTransformMatrixParameters();
}



void GUIObject::move(glm::vec3 delta)
{
    _position += delta;

    changedVerticesTransformMatrixParameters();
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

    changedVerticesTransformMatrixParameters();
}


void GUIObject::scale(float x, float y)
{
    scale(glm::vec2(x, y));
}


void GUIObject::rotate(float angle)
{
    _rotation += angle;

    changedVerticesTransformMatrixParameters();
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


void GUIObject::setIsActive(bool isActive)
{
    _isActive = isActive;
}


bool GUIObject::isActive()
{
    return _isActive;
}


void GUIObject::setColor(glm::vec4 color)
{
    _color = color;
}


glm::vec4 GUIObject::getColor()
{
    return _color;
}


VBO* GUIObject::getVBO(unsigned int index)
{
    return _vbos[index];
}


unsigned int GUIObject::getQuantumOfVBO()
{
    return _vbos.size();
}
