#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "RotationMode.h"

#include "../Scripting/Utils/LuaMacros.h"


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

        LUAF void setPosition(glm::vec3 position);                               // Set in local space
        LUAF void setPosition(float x, float y, float z);
        LUAF void setRotation(glm::vec3 rotation);                               // Rotations in radians
        LUAF void setRotation(float x, float y, float z);
        void setRotationQuaternion(glm::quat rotation);
        LUAR void setRotationQuaternion(float x, float y, float z, float w);
        LUAF void setScale(glm::vec3 scale);
        LUAF void setScale(float x, float y, float z);
        LUAF void setScale(float scale);

        LUAF void move(glm::vec3 deltaPosition);
        LUAF void move(float dx, float dy, float dz);
        LUAF void rotate(glm::vec3 deltaRotation);
        LUAF void rotate(float dx, float dy, float dz);
        LUAF void scale(glm::vec3 scale);
        LUAF void scale(float x, float y, float z);

        LUAF glm::vec3 getPosition() const;                                      // In local space
        LUAF glm::vec3 getRotation() const;
        glm::quat getRotationQuaternion() const;
        LUAF glm::vec3 getScale() const;

        glm::mat4& getTransformMatrix() const;
        glm::mat4& getNormalMatrix() const;

        Transform& operator=(const Transform& t);

};


#endif // TRANSFORM_H_INCLUDED
