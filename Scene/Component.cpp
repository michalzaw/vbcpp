#include "Component.h"

#include "SceneObject.h"


Component::Component(ComponentType type)
    : _type(type),
    _object(NULL),// _objectTransform(NULL),
    _isActive(true)
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

        //_objectTransform = _object->getTransform();
    }
}


void Component::setIsActive(bool is)
{
    _isActive = is;
}


ComponentType Component::getType()
{
    return _type;
}


SceneObject* Component::getSceneObject()
{
    return _object;
}


bool Component::isActive()
{
    return _isActive && _object->isActive();
}
