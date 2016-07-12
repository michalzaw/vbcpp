#include "Component.h"

#include "SceneObject.h"


Component::Component(ComponentType type)
    : _type(type),
    _object(NULL), _objectTransform(NULL)
{
    #ifdef _DEBUG_MODE
        std::cout << "Create Component\n";
    #endif // _DEBUG_MODE
}


Component::~Component()
{
    #ifdef _DEBUG_MODE
        std::cout << "Destroy Component\n";
    #endif // _DEBUG_MODE
}


void Component::setSceneObject(SceneObject* object)
{
    if (object != NULL)
    {
        _object = object;

        _objectTransform = _object->getTransform();
    }
}


ComponentType Component::getType()
{
    return _type;
}


SceneObject* Component::getSceneObject()
{
    return _object;
}


Transform* Component::getTransform()
{
    return _objectTransform;
}
