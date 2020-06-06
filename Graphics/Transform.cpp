#include "Transform.h"

#include "../Scene/SceneObject.h"

#include <cstdio>


// Implementations of all functions are copies of functions from SceneObject


Transform::Transform()
    : _rotationMode(RM_QUATERNION),
    _position(0.0f, 0.0f, 0.0f),
    _rotation(0.0f, 0.0f, 0.0f),
    _scale(1.0f, 1.0f, 1.0f)
{
    changed();
}


Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : _rotationMode(RM_EULER_ANGLES),
    _position(position),
    _rotation(rotation),
    _scale(scale)
{
    changed();
}


Transform::Transform(const Transform& t)
{
    _rotationMode = t._rotationMode;

    _position = t._position;
    _rotation = t._rotation;
    _rotationQuaternion = t._rotationQuaternion;
    _scale = t._scale;

    changed();
}


Transform::~Transform()
{

}


void Transform::updateTransformMatrix() const
{
    glm::mat4 pos = glm::translate(_position);

    glm::mat4 rot;

    if (_rotationMode == RM_EULER_ANGLES)
    {
        rot = glm::rotate(_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        rot *= glm::rotate(_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rot *= glm::rotate(_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    else
    {
        rot = glm::mat4_cast(_rotationQuaternion);
    }

    glm::mat4 s = glm::scale(_scale);

    _transformMatrix = pos * rot * s;
}

void Transform::changed()
{
    _transformMatrixIsCalculated = false;
    _normalMatrixIsCalculated = false;
}


void Transform::setPosition(glm::vec3 position)
{
    _position = position;

    changed();
}


void Transform::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x, y, z));
}


void Transform::setRotation(glm::vec3 rotation)
{
    _rotation = rotation;
    _rotationQuaternion = glm::quat(rotation);

    _rotationMode = RM_EULER_ANGLES;

    changed();
}


void Transform::setRotation(float x, float y, float z)
{
    setRotation(glm::vec3(x, y, z));
}


void Transform::setRotationQuaternion(glm::quat rotation)
{
    _rotationQuaternion = rotation;
    _rotation = glm::eulerAngles(rotation);

    _rotationMode = RM_QUATERNION;

    changed();
}


void Transform::setRotationQuaternion(float x, float y, float z, float w)
{
    _rotationQuaternion.x = x;
    _rotationQuaternion.y = y;
    _rotationQuaternion.z = z;
    _rotationQuaternion.w = w;

    _rotationMode = RM_QUATERNION;

    changed();
}


void Transform::setScale(glm::vec3 scale)
{
    _scale = scale;

    changed();
}


void Transform::setScale(float x, float y, float z)
{
    setScale(glm::vec3(x, y, z));
}


void Transform::move(glm::vec3 deltaPosition)
{
    _position += deltaPosition;

    changed();
}


void Transform::move(float dx, float dy, float dz)
{
    move(glm::vec3(dx, dy, dz));
}


void Transform::rotate(glm::vec3 deltaRotation)
{
    _rotation += deltaRotation;

    _rotationMode = RM_EULER_ANGLES;

    changed();
}


void Transform::rotate(float dx, float dy, float dz)
{
    rotate(glm::vec3(dx, dy, dz));
}


void Transform::scale(glm::vec3 scale)
{
    _scale *= scale;

    changed();
}


void Transform::scale(float x, float y, float z)
{
    scale(glm::vec3(x, y, z));
}


glm::vec3 Transform::getPosition() const
{
    return _position;
}


glm::vec3 Transform::getRotation() const
{
    return _rotation;
}


glm::quat Transform::getRotationQuaternion() const
{
    return _rotationQuaternion;
}


glm::vec3 Transform::getScale() const
{
    return _scale;
}


glm::mat4& Transform::getTransformMatrix() const
{
    if (!_transformMatrixIsCalculated)
    {
        updateTransformMatrix();

        _transformMatrixIsCalculated = true;
    }

    return _transformMatrix;
}


glm::mat4& Transform::getNormalMatrix() const
{
    if (!_normalMatrixIsCalculated)
    {
        _normalMatrix = glm::transpose(glm::inverse(getTransformMatrix()));

        _normalMatrixIsCalculated = true;
    }

    return _normalMatrix;
}


Transform& Transform::operator=(const Transform& t)
{
    _rotationMode = t._rotationMode;

    _position = t._position;
    _rotation = t._rotation;
    _rotationQuaternion = t._rotationQuaternion;
    _scale = t._scale;

    changed();

    return *this;
}
