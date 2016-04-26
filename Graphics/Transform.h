#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>


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

        void  UpdateTransformMatrix();
        void Changed();

    public:
        Transform();
        Transform(const Transform& t);
        ~Transform();

        void SetPosition(glm::vec3 position);
        void SetRotation(glm::vec3 rotation);
        void SetRotation(float x, float y, float z, float w);

        void SetScale(glm::vec3 scale);
        glm::vec3 GetPosition();
        glm::vec3 GetRotation();
        glm::vec3 GetScale();

        glm::mat4& GetTransformMatrix();
        glm::mat4& GetNormalMatrix();

        Transform& operator=(const Transform& t);

};


#endif // TRANSFORM_H_INCLUDED
