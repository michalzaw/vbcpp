#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED


#include <list>
#include <map>

#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Roads.h"
#include "../Graphics/TreeComponent.h"
#include "../Physics/PhysicsManager.hpp"
#include "SoundManager.h"
#include "SceneObject.h"

#include "../Game/BusStopSystem.h"


class GameLogicSystem;
class BusStopSystem;


class SceneManager
{
    private:
        GraphicsManager*    _graphicsManager;
        PhysicsManager*     _physicsManager;
        SoundManager*       _soundManager;
        GameLogicSystem*    _gameLogicSystem;
        BusStopSystem*      _busStopSystem;

        std::list<SceneObject*> _sceneObjects;

    public:
        SceneManager(GraphicsManager* gMgr, PhysicsManager* pMgr, SoundManager* sndMgr);
        ~SceneManager();

        GraphicsManager*    getGraphicsManager() { return _graphicsManager; };
        PhysicsManager*     getPhysicsManager() { return _physicsManager; };
        SoundManager*       getSoundManager() { return _soundManager; };
        GameLogicSystem*    getGameLogicSystem() { return _gameLogicSystem; };
        BusStopSystem*      getBusStopSystem() { return _busStopSystem; };


        SceneObject*    addSceneObject(std::string name, RObject* objectDefinition = NULL);
        void            removeSceneObject(SceneObject* object, bool removeChildren = true);
        void            removeChildSceneObject(SceneObject* object, bool removeChildren = true);

        void            removeSceneObject(std::string name, bool removeChildren = true);

        void clearScene()
        {
            for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
            {
                delete *i;
            }

            _sceneObjects.clear();

            _graphicsManager->clearQuadTree();
        }

        SceneObject*    getSceneObject(std::string name);
        std::list<SceneObject*>& getSceneObjects();

};


#endif // SCENEMANAGER_H_INCLUDED
