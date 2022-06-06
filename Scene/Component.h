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
    CT_GRASS,
	CT_ROAD_OBJECT,
	CT_CROSSROAD,
    CT_ROAD_INTERSECTION,
	CT_TERRAIN,
	CT_SKY,
    CT_ENVIRONMENT_CAPTURE_COMPONENT,
    CT_MIRROR,
    CT_CLICKABLE_OBJECT,
    CT_SHAPE_POLYGON,
    CT_SKELETAL_ANIMATION,

    CT_BUS_STOP,
	CT_DISPLAY,
	CT_CAMERA_CONTROL,

    COMPONENTS_TYPE_SIZE
};


class Component
{
    protected:
        ComponentType _type;

        SceneObject* _object;
//        Transform* _objectTransform;

        bool _isActive;

        virtual void onAttachedToScenObject() {}

    public:
        Component(ComponentType type);
        ~Component();

        void setSceneObject(SceneObject* object);
        void setIsActive(bool is);

        ComponentType   getType();
        SceneObject*    getSceneObject();
        bool            isActive();

        virtual void changedTransform() {}
		virtual void update(float deltaTime) {}

};


#endif // COMPONENT_H_INCLUDED
