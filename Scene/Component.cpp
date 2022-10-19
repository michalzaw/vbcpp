#include "Component.h"

#include "SceneObject.h"

#include "../Utils/Logger.h"


Component::Component(ComponentType type)
    : _type(type),
    _object(NULL),// _objectTransform(NULL),
    _isActive(true)
{
    LOG_DEBUG("Create Component");
}


Component::~Component()
{
    LOG_DEBUG("Destroy Component");
}


void Component::setSceneObject(SceneObject* object)
{
    if (object != NULL)
    {
        _object = object;

        onAttachedToScenObject();
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
