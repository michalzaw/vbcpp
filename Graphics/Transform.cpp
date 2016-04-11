#include "Transform.h"


void Transform::UpdateTransformMatrix()
{
    _transformMatrix = glm::translate(_position) *
                       glm::rotate(_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
                       glm::rotate(_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
                       glm::rotate(_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
                       glm::scale(_scale);

    _transformMatrixIs = true;
}

void Transform::Changed()
{
    _transformMatrixIs = false;
    _normalMatrixIs = false;
}


Transform::Transform()
    : _position(0.0f, 0.0f, 0.0f),
    _rotation(0.0f, 0.0f, 0.0f),
    _scale(1.0f, 1.0f, 1.0f)
{
    Changed();
}


Transform::Transform(const Transform& t)
{
    _position = t._position;
    _rotation = t._rotation;
    _scale = t._scale;

    Changed();
}


Transform::~Transform()
{

}


void Transform::SetPosition(glm::vec3 position)
{
    _position = position;

    Changed();
}


void Transform::SetRotation(glm::vec3 rotation)
{
    _rotation = rotation;

    Changed();
}


void Transform::SetScale(glm::vec3 scale)
{
    _scale = scale;

    Changed();
}


glm::vec3 Transform::GetPosition()
{
    return _position;
}


glm::vec3 Transform::GetRotation()
{
    return _rotation;
}


glm::vec3 Transform::GetScale()
{
    return _scale;
}


glm::mat4& Transform::GetTransformMatrix()
{
    if (!_transformMatrixIs)
    {
        UpdateTransformMatrix();
    }

    return _transformMatrix;
}


glm::mat4& Transform::GetNormalMatrix()
{
    if (!_normalMatrixIs)
    {
        _normalMatrix = glm::transpose(glm::inverse(GetTransformMatrix()));

        _normalMatrixIs = true;
    }

    return _normalMatrix;
}


Transform& Transform::operator=(const Transform& t)
{
    _position = t._position;
    _rotation = t._rotation;
    _scale = t._scale;

    Changed();

    return *this;
}
