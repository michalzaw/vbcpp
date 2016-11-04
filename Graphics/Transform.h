#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>


class SceneObject;


class Transform
{
    private:
        glm::vec3 _position;
        glm::vec3 _rotation;
        glm::vec3 _scale;

        // TODO Inne macierze np. odwrotna
        glm::mat4 _transformMatrix;
        glm::mat4 _normalMatrix;
        bool _transformMatrixIs;
        bool _normalMatrixIs;

        SceneObject* _object;

        void updateTransformMatrix();
        void changed();

    public:
        Transform(SceneObject* object);
        Transform(const Transform& t);
        ~Transform();

        void setPosition(glm::vec3 position);
        void setRotation(glm::vec3 rotation);
        void setRotation(float x, float y, float z, float w);
        void setScale(glm::vec3 scale);

        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getScale();

        glm::mat4& getTransformMatrix();
        glm::mat4& getNormalMatrix();

        Transform& operator=(const Transform& t);

};


#endif // TRANSFORM_H_INCLUDED
