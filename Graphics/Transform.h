#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>


/*class SceneObject;


enum RotationMode
{
    RM_EULER_ANGLES,
    RM_QUATERNION

};


class Transform
{
    private:
        RotationMode _rotationMode;

        glm::vec3 _position;
        glm::vec3 _rotation;
        glm::quat _rotationQuaternion;
        glm::vec3 _scale;

        // TODO Inne macierze np. odwrotna
        mutable glm::mat4 _transformMatrix;
        mutable glm::mat4 _normalMatrix;
        mutable bool _transformMatrixIs;
        mutable bool _normalMatrixIs;

        SceneObject* _object;

        void updateTransformMatrix() const;
        void changed();

    public:
        Transform(SceneObject* object);
        Transform(const Transform& t);
        ~Transform();

        void setPosition(glm::vec3 position);
        void setRotation(glm::vec3 rotation);
        void setRotation(float x, float y, float z, float w);
        void setScale(glm::vec3 scale);

        glm::vec3 getPosition() const;
        glm::vec3 getRotation() const;
        glm::vec3 getScale() const;

        glm::mat4& getTransformMatrix() const;
        glm::mat4& getNormalMatrix() const;

        Transform& operator=(const Transform& t);
        Transform& operator*=(const Transform& t);

        void setObject(SceneObject* object)
        {
            _object = object;
        }

};*/


#endif // TRANSFORM_H_INCLUDED
