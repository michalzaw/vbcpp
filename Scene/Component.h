#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED


#ifdef _DEBUG_MODE
#include <iostream>
#endif // _DEBUG_MODE

#include "../Graphics/Transform.h"


class SceneObject;


enum ComponentType
{
    CT_RENDER_OBJECT,
    CT_CAMERA,
    CT_LIGHT,
    CT_PHYSICAL_BODY

};


class Component
{
    protected:
        ComponentType _type;

        SceneObject* _object;
//        Transform* _objectTransform;

        bool _isActive;

    public:
        Component(ComponentType type);
        ~Component();

        void setSceneObject(SceneObject* object);
        void setIsActive(bool is);

        ComponentType   getType();
        SceneObject*    getSceneObject();
        bool            isActive();

        //virtual void changedTransform(glm::vec3 position = glm::vec3(0,0,0), glm::vec3 rotation = glm::vec3(0,0,0)) {}
        virtual void changedTransform(glm::mat4 localMatrix = glm::mat4(1.0f)) {}
};


#endif // COMPONENT_H_INCLUDED
