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
    CT_PHYSICAL_BODY,
    CT_TREE_COMPONENT,
    CT_SOUND,
    CT_GRASS
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

        virtual void changedTransform() {}

};


#endif // COMPONENT_H_INCLUDED
