#include "SceneObject.h"
#include "SceneManager.h"


SceneObject::SceneObject(std::string name, SceneManager* sceneManager, SceneObject* parent)
    : _parent(parent),
    _id(0), _name(name), _isActive(true),
    _sceneManager(sceneManager),
    _transform(this), _globalTransform(this), _transformIsChanged(true)
{
    if (_parent != NULL)
    {
        _parent->addChild(this);
    }

    //#ifdef _DEBUG_MODE
        std::cout << "Create SceneObject: " << _name << std::endl ;
    //#endif // _DEBUG_MODE
}


SceneObject::~SceneObject()
{
    //#ifdef _DEBUG_MODE
        std::cout << "Destroy SceneObject: " << _name << std::endl;
    //#endif // _DEBUG_MODE

    for (std::vector<Component*>:: iterator i = _components.begin(); i != _components.end(); ++i)
    {
        /* USUWANIE Z POSZCZEGOLNYCH PODSYSTEMOW */
        switch ((*i)->getType())
        {
            case CT_RENDER_OBJECT:
                //_sceneManager->getGraphicsManager()->removeRenderObject(static_cast<RenderObject*>(*i));
                GraphicsManager::getInstance().removeRenderObject(static_cast<RenderObject*>(*i));
                break;

            case CT_CAMERA:
                //_sceneManager->getGraphicsManager()->removeCamera(static_cast<CameraStatic*>(*i));
                GraphicsManager::getInstance().removeCamera(static_cast<CameraStatic*>(*i));
                break;

            case CT_LIGHT:
                //_sceneManager->getGraphicsManager()->removeLight(static_cast<Light*>(*i));
                GraphicsManager::getInstance().removeLight(static_cast<Light*>(*i));
                break;

            case CT_PHYSICAL_BODY:
                //_sceneManager->getPhysicsManager()->removePhysicalBody(static_cast<PhysicalBody*>(*i));
                //PhysicsManager::getInstance().removePhysicalBody(static_cast<PhysicalBody*>(*i));
                break;

        }

        /* ------------------------------------- */
    }
}


void SceneObject::calculateGlobalTransform()
{
    _globalTransform = _transform;

    if (_parent != NULL)
    {
        _globalTransform *= *(_parent->getGlobalTransform());
        //_globalTransform = *(_parent->getGlobalTransform()) * _globalTransform;
        //_globalTransform.setObject(this);
    }
}


void SceneObject::removeParent()
{
    if (_parent != NULL)
    {
        _parent->removeChild(this);
        _parent = NULL;
    }
}


bool SceneObject::hasParent()
{
    return static_cast<bool>(_parent);
}


SceneObject* SceneObject::getParent()
{
    return _parent;
}


void SceneObject::addChild(SceneObject* child)
{
    if (child == NULL)
    {
        return;
    }

    _childrens.push_back(child);

    child->removeParent();
    child->_parent = this;

    child->setIsActive(_isActive);
}


bool SceneObject::removeChild(SceneObject* child)
{
    if (child == NULL)
    {
        return false;
    }

    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        if ((*i) == child)
        {
            child->_parent = NULL;
            _childrens.erase(i);

            return true;
        }
    }

    return false;
}


void SceneObject::removeAllChildren()
{
    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        (*i)->_parent = NULL;
    }

    _childrens.clear();
}


void SceneObject::addComponent(Component* component)
{
    if (component != NULL)
    {
        component->setSceneObject(this);

        _components.push_back(component);
    }
}


void SceneObject::removeComponent(Component* component)
{
    for (std::vector<Component*>::iterator i = _components.begin(); i != _components.end(); ++i)
    {
        if (*i == component)
        {
            i = _components.erase(i);

            /* USUWANIE Z POSZCZEGOLNYCH PODSYSTEMOW */
            switch ((*i)->getType())
            {
                case CT_RENDER_OBJECT:
                    //_sceneManager->getGraphicsManager()->removeRenderObject(static_cast<RenderObject*>(*i));
                    GraphicsManager::getInstance().removeRenderObject(static_cast<RenderObject*>(*i));
                    break;

                case CT_CAMERA:
                    //_sceneManager->getGraphicsManager()->removeCamera(static_cast<CameraStatic*>(*i));
                    GraphicsManager::getInstance().removeCamera(static_cast<CameraStatic*>(*i));
                    break;

                case CT_LIGHT:
                    //_sceneManager->getGraphicsManager()->removeLight(static_cast<Light*>(*i));
                    GraphicsManager::getInstance().removeLight(static_cast<Light*>(*i));
                    break;

                case CT_PHYSICAL_BODY:
                    //_sceneManager->getPhysicsManager()->removePhysicalBody(static_cast<PhysicalBody*>(*i));
                    //PhysicsManager::getInstance().removePhysicalBody(static_cast<PhysicalBody*>(*i));
                    break;

            }

            /* ------------------------------------- */

            return;
        }
    }
}


void SceneObject::setIsActive(bool is)
{
    _isActive = is;

    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        (*i)->setIsActive(_isActive);
    }
}


std::string SceneObject::getName()
{
    return _name;
}

unsigned int SceneObject::getId()
{
    return _id;
}


Transform* SceneObject::getTransform()
{
    return &_transform;
}


Transform* SceneObject::getGlobalTransform()
{
    if (_transformIsChanged)
    {
        calculateGlobalTransform();

        _transformIsChanged = false;
    }

    return &_globalTransform;
}


Component* SceneObject::getComponent(unsigned int index)
{
    if (index < _components.size())
        return _components[index];

    return NULL;
}


bool SceneObject::isActive()
{
    return _isActive;
}


unsigned int SceneObject::getComponentsCount()
{
    return _components.size();
}


SceneManager* SceneObject::getSceneManager()
{
    return _sceneManager;
}


void SceneObject::changedTransform()
{
    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        (*i)->changedTransform();
    }

    for (std::vector<Component*>::iterator i = _components.begin(); i != _components.end(); ++i)
    {
        (*i)->changedTransform();
    }

    _transformIsChanged = true;
}
