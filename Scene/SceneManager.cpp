#include "SceneManager.h"

#include "../Game/GameLogicSystem.h"


SceneManager::SceneManager(GraphicsManager* gMgr, PhysicsManager* pMgr, SoundManager* sndMgr)
    : _graphicsManager(gMgr), _physicsManager(pMgr), _soundManager(sndMgr),
    _nextObjectId(MIN_OBJECT_ID)
{
    LOG_DEBUG("Create SceneManager");
    _physicsManager->grab();
    _soundManager->grab();

    _gameLogicSystem = new GameLogicSystem;
    _busStopSystem = new BusStopSystem;

    //_graphicsManager = new GraphicsManager;
    //_physicsManager = new PhysicsManager;

    //_physicsManager->createPhysicsWorld();
}


SceneManager::~SceneManager()
{
    LOG_DEBUG("Destroy SceneManager");


    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        delete *i;
    }

    _physicsManager->drop();
    _soundManager->drop();
    delete _graphicsManager;

    delete _gameLogicSystem;
    delete _busStopSystem;

    //delete _graphicsManager;
    //delete _physicsManager;
}


ObjectId SceneManager::generateNewObjectId()
{
    while (_sceneObjectsMap.find(_nextObjectId) != _sceneObjectsMap.end())
    {
        _nextObjectId = clamp(_nextObjectId + 1, MIN_OBJECT_ID, MAX_OBJECT_ID);
    }

    return _nextObjectId;
}


SceneObject* SceneManager::addSceneObject(const std::string& name, ObjectId id/* = 0u*/, RObject* objectDefinition/* = nullptr*/)
{
    ObjectId objectId = id;
    if (objectId == 0u)
    {
        objectId = generateNewObjectId();
    }

    auto existingSceneObject = _sceneObjectsMap.find(objectId);
    if (existingSceneObject != _sceneObjectsMap.end())
    {
        LOG_ERROR("Object with id=" + Strings::toString(objectId) + " already exist.");
        return nullptr;
    }

    SceneObject* obj = new SceneObject(name, objectId, this, objectDefinition);

    _sceneObjects.push_back(obj);
    _sceneObjectsMap.insert(std::make_pair(obj->getId(), obj));

    return obj;
}


void SceneManager::removeSceneObject(SceneObject* object, bool removeChildren/* = true*/)
{
    _sceneObjectsMap.erase(object->getId());

    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if (*i == object)
        {
            i = _sceneObjects.erase(i);

            if (removeChildren)
            {
                for (std::list<SceneObject*>::const_iterator j = object->getChildren().begin(); j != object->getChildren().end(); ++j)
                {
                    removeChildSceneObject(*j);
                }
            }
            else
            {
                object->removeAllChildren();
            }

            object->removeParent();
            delete object;

            return;
        }
    }
}


void SceneManager::removeChildSceneObject(SceneObject* object, bool removeChildren/* = true*/)
{
    _sceneObjectsMap.erase(object->getId());

    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if (*i == object)
        {
            i = _sceneObjects.erase(i);

            if (removeChildren)
            {
                for (std::list<SceneObject*>::const_iterator j = object->getChildren().begin(); j != object->getChildren().end(); ++j)
                {
                    removeChildSceneObject(*j);
                }
            }
            else
            {
                object->removeAllChildren();
            }

            delete object;

            return;
        }
    }
}


void SceneManager::removeSceneObject(const std::string& name, bool removeChildren/* = true*/)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end();)
    {
        if ((*i)->getName() == name)
        {
            SceneObject* temp = *i;

            _sceneObjectsMap.erase(temp->getId());

            i = _sceneObjects.erase(i);

            if (removeChildren)
            {
                for (std::list<SceneObject*>::const_iterator j = temp->getChildren().begin(); j != temp->getChildren().end(); ++j)
                {
                    removeChildSceneObject(*j);
                }
            }
            else
            {
                temp->removeAllChildren();
            }

            temp->removeParent();
            delete temp;
        }
        else
        {
            ++i;
        }
    }
}


void SceneManager::removeSceneObject(ObjectId id, bool removeChildren/* = true*/)
{
    auto object = _sceneObjectsMap.find(id);
    if (object != _sceneObjectsMap.end())
    {
        removeSceneObject(object->second, removeChildren);
    }
}


void SceneManager::clearScene()
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        delete* i;
    }

    _sceneObjects.clear();

    _graphicsManager->clearQuadTree();
}


SceneObject* SceneManager::getSceneObject(const std::string& name)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if ((*i)->getName() == name)
            return *i;
    }

    return 0;
}


SceneObject* SceneManager::getSceneObject(ObjectId id)
{
    auto object = _sceneObjectsMap.find(id);
    if (object != _sceneObjectsMap.end())
    {
        return object->second;
    }

    LOG_ERROR("Cannot find Object with id: " + Strings::toString(id));
    return nullptr;
}


std::list<SceneObject*>& SceneManager::getSceneObjects()
{
    return _sceneObjects;
}
