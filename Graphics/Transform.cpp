#include "Transform.h"

#include "../Scene/SceneObject.h"

#include <cstdio>


void Transform::updateTransformMatrix() const
{
    glm::mat4 pos = glm::translate(_position);

    /*
    _transformMatrix = glm::translate(_position) *
                       glm::rotate(_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
                       glm::rotate(_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
                       glm::rotate(_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
                       glm::scale(_scale);
    */

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

    _transformMatrixIs = true;
}

void Transform::changed()
{
    _transformMatrixIs = false;
    _normalMatrixIs = false;

    _object->changedTransform();
}


Transform::Transform(SceneObject* object)
    : _rotationMode(RM_QUATERNION),
    _position(0.0f, 0.0f, 0.0f),
    _rotation(0.0f, 0.0f, 0.0f),
    _scale(1.0f, 1.0f, 1.0f),
    _object(object)
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

    _object = t._object;

    changed();
}


Transform::~Transform()
{

}


void Transform::setPosition(glm::vec3 position)
{
    _position = position;

    changed();
}


void Transform::setRotation(glm::vec3 rotation)
{
    _rotation = rotation;

    changed();

    _rotationMode = RM_EULER_ANGLES;
}


void Transform::setRotation(float x, float y, float z, float w)
{
    _rotationQuaternion.x = x;
    _rotationQuaternion.y = y;
    _rotationQuaternion.z = z;
    _rotationQuaternion.w = w;

    changed();

    _rotationMode = RM_QUATERNION;

    /*glm::quat q;
    q.x = x;
    q.y = y;
    q.z = z;
    q.w = w;
    glm::mat4 rot = glm::mat4_cast(q);

    _transformMatrix = glm::translate(_position) * rot * glm::scale(_scale);

    changed();*/
}


void Transform::setScale(glm::vec3 scale)
{
    _scale = scale;

    changed();
}


glm::vec3 Transform::getPosition() const
{
    return _position;
}


glm::vec3 Transform::getRotation() const
{
    return _rotation;
}


glm::vec3 Transform::getScale() const
{
    return _scale;
}


glm::mat4& Transform::getTransformMatrix() const
{
    if (!_transformMatrixIs)
    {
        updateTransformMatrix();
    }

    return _transformMatrix;
}


glm::mat4& Transform::getNormalMatrix() const
{
    if (!_normalMatrixIs)
    {
        _normalMatrix = glm::transpose(glm::inverse(getTransformMatrix()));

        _normalMatrixIs = true;
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

    _object = t._object;

    changed();
    //_transformMatrix = t._transformMatrix;
    //_normalMatrix = t._normalMatrix;

    return *this;
}


Transform& Transform::operator*=(const Transform& t)
{
    glm::mat4 transformMatrix = getTransformMatrix();
    glm::mat4 transformMatrix2 = t.getTransformMatrix();
    _transformMatrix = transformMatrix2 * transformMatrix;      // Zmienic sposob mnozenia macierzy

    _transformMatrixIs = true;
    _normalMatrixIs = false;

    return *this;
}
