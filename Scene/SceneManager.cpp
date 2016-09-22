#include "SceneManager.h"


SceneManager::SceneManager()
{
    #ifdef _DEBUG_MODE
        std::cout << "Create SceneManager\n";
    #endif // _DEBUG_MODE


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


    //delete _graphicsManager;
    //delete _physicsManager;
}

/*
GraphicsManager* SceneManager::getGraphicsManager()
{
    return _graphicsManager;
}
*/
/*
PhysicsManager* SceneManager::getPhysicsManager()
{
    return _physicsManager;
}
*/

SceneObject* SceneManager::addSceneObject(std::string name)
{
    SceneObject* obj = new SceneObject(name, this);

    _sceneObjects.push_back(obj);

    return obj;
}


void SceneManager::removeSceneObject(SceneObject* object)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if (*i == object)
        {
            i = _sceneObjects.erase(i);

            delete object;

            return;
        }
    }
}

void SceneManager::removeSceneObject(std::string name)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if ((*i)->getName() == name)
        {
            delete *i;

            i = _sceneObjects.erase(i);


        }
    }
}

SceneObject* SceneManager::getSceneObject(std::string name)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if ((*i)->getName() == name)
            delete *i;
    }
}
