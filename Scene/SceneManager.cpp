#include "SceneManager.h"

#include "../Game/GameLogicSystem.h"


SceneManager::SceneManager(GraphicsManager* gMgr, PhysicsManager* pMgr, SoundManager* sndMgr)
    : _graphicsManager(gMgr), _physicsManager(pMgr), _soundManager(sndMgr)
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


SceneObject* SceneManager::addSceneObject(const std::string& name, RObject* objectDefinition/* = nullptr*/)
{
    SceneObject* obj = new SceneObject(name, this, objectDefinition);

    _sceneObjects.push_back(obj);

    return obj;
}


void SceneManager::removeSceneObject(SceneObject* object, bool removeChildren/* = true*/)
{
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

            i = _sceneObjects.erase(i);

            if (removeChildren)
            {
                for (std::list<SceneObject*>::const_iterator j = temp->getChildren().begin(); j != temp->getChildren().end(); ++j)
                {
                    removeChildSceneObject(*j);
                }
            }

            delete temp;
        }
        else
        {
            ++i;
        }
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


std::list<SceneObject*>& SceneManager::getSceneObjects()
{
    return _sceneObjects;
}
