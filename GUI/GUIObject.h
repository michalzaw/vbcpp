#ifndef GUIOBJECT_H_INCLUDED
#define GUIOBJECT_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Graphics/OGLDriver.h"
#include "../Graphics/VBO.h"


struct GUIVertex
{
    glm::vec3 position;
    glm::vec2 texCoord;

};


class GUIObject
{
    protected:
        glm::vec3   _position;
        glm::vec2   _scale;
        float       _rotation;

        glm::vec2   _size;                      // Wykorzystywane wewnetrznie, zalezy np od rozmiaru tekstury

        glm::mat4 _verticesTransformMatrix;
        glm::mat4 _texCoordTransformMatrix;

        bool _verticesTransformMatrixIs;
        bool _texCoordTransformMatrixIs;

        bool _isActive;

        VBO* _vbo;


        void setSize(glm::vec2 _size);


        virtual void calculateVerticesTransformMatrix();
        virtual void calculateTexCoordTransformMatrix();

    public:
        GUIObject();
        ~GUIObject();

        void setPosition(glm::vec3 position);
        void setPosition(glm::vec2 position);
        void setPosition(float x, float y, float z = 0.0f);
        void setScale(glm::vec2 scale);
        void setScale(float scaleX, float scaleY);
        void setRotation(float angle);          // Radians

        void move(glm::vec3 delta);
        void move(glm::vec2 delta);
        void move(float x, float y, float z = 0.0f);
        void scale(glm::vec2 delta);
        void scale(float x, float y);
        void rotate(float angle);

        glm::vec3   getPosition();
        glm::vec2   getScale();
        float       getRotation();


        glm::vec2   getSize();


        void setIsActive(bool isActive);
        bool isActive();


        VBO* getVBO();

        glm::mat4& getVerticesTransformMatrix();
        glm::mat4& getTexCoordTransformMatrix();

};


#endif // GUIOBJECT_H_INCLUDED
