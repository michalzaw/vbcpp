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
        Transform* _objectTransform;

    public:
        Component(ComponentType type);
        ~Component();

        void setSceneObject(SceneObject* object);

        ComponentType   getType();
        SceneObject*    getSceneObject();
        Transform*      getTransform();

        virtual void changedTransform() {}

};


#endif // COMPONENT_H_INCLUDED
