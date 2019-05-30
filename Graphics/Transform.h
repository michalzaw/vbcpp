#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "RotationMode.h"


class Transform
{
    private:
        RotationMode _rotationMode;

        glm::vec3 _position;
        glm::vec3 _rotation;
        glm::quat _rotationQuaternion;
        glm::vec3 _scale;

        mutable glm::mat4 _transformMatrix;
        mutable glm::mat4 _normalMatrix;
        mutable bool _transformMatrixIsCalculated;
        mutable bool _normalMatrixIsCalculated;

        void updateTransformMatrix() const;
        void changed();

    public:
        Transform();
        Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
        Transform(const Transform& t);
        ~Transform();

        void setPosition(glm::vec3 position);                               // Set in local space
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 rotation);                               // Rotations in radians
        void setRotation(float x, float y, float z);
        void setRotationQuaternion(glm::quat rotation);
        void setRotationQuaternion(float x, float y, float z, float w);
        void setScale(glm::vec3 scale);
        void setScale(float x, float y, float z);
        void setScale(float scale);

        void move(glm::vec3 deltaPosition);
        void move(float dx, float dy, float dz);
        void rotate(glm::vec3 deltaRotation);
        void rotate(float dx, float dy, float dz);
        void scale(glm::vec3 scale);
        void scale(float x, float y, float z);

        glm::vec3 getPosition() const;                                      // In local space
        glm::vec3 getRotation() const;
        glm::quat getRotationQuaternion() const;
        glm::vec3 getScale() const;

        glm::mat4& getTransformMatrix() const;
        glm::mat4& getNormalMatrix() const;

        Transform& operator=(const Transform& t);

};


#endif // TRANSFORM_H_INCLUDED
