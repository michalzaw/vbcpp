#include "SceneManager.h"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include "../Utils/Helpers.hpp"
#include "../Utils/XmlUtils.h"
#include "../Graphics/LoadTerrainModel.h"

#include "../Game/Directories.h"

SceneManager::SceneManager(GraphicsManager* gMgr, PhysicsManager* pMgr, SoundManager* sndMgr)
    : _graphicsManager(gMgr), _physicsManager(pMgr), _soundManager(sndMgr)
{
    #ifdef _DEBUG_MODE
        std::cout << "Create SceneManager\n";
    #endif // _DEBUG_MODE
    _physicsManager->grab();
    _soundManager->grab();

    _busStart.position = glm::vec3(0,3,0);
    _busStart.rotation = glm::vec3(0,0,0);

    //_graphicsManager = new GraphicsManager;
    //_physicsManager = new PhysicsManager;

    //_physicsManager->createPhysicsWorld();
}


SceneManager::~SceneManager()
{
    #ifdef _DEBUG_MODE
        std::cout << "Destroy SceneManager\n";
    #endif // _DEBUG_MODE


    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        delete *i;
    }

    _physicsManager->drop();
    _soundManager->drop();

    //delete _graphicsManager;
    //delete _physicsManager;
}


SceneObject* SceneManager::addSceneObject(std::string name, RObject* objectDefinition)
{
    SceneObject* obj = new SceneObject(name, this, objectDefinition);

    _sceneObjects.push_back(obj);

    return obj;
}


void SceneManager::removeSceneObject(SceneObject* object, bool removeChildren)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if (*i == object)
        {
            i = _sceneObjects.erase(i);

            if (removeChildren)
            {
                for (std::list<SceneObject*>::iterator j = object->getChildren().begin(); j != object->getChildren().end(); ++j)
                {
                    removeSceneObject(*j);
                }
            }

            delete object;

            return;
        }
    }
}

void SceneManager::removeSceneObject(std::string name, bool removeChildren)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end();)
    {
        if ((*i)->getName() == name)
        {
            SceneObject* temp = *i;

            i = _sceneObjects.erase(i);

            if (removeChildren)
            {
                for (std::list<SceneObject*>::iterator j = temp->getChildren().begin(); j != temp->getChildren().end(); ++j)
                {
                    removeSceneObject(*j);
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

SceneObject* SceneManager::getSceneObject(std::string name)
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
