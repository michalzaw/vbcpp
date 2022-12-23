#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED


#include <list>

#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Roads.h"
#include "../Graphics/TreeComponent.h"
#include "../Physics/PhysicsManager.hpp"
#include "SoundManager.h"
#include "SceneObject.h"

#include "../Game/BusStopSystem.h"


class GameLogicSystem;


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

        inline GraphicsManager*    getGraphicsManager() { return _graphicsManager; };
        inline PhysicsManager*     getPhysicsManager() { return _physicsManager; };
        inline SoundManager*       getSoundManager() { return _soundManager; };
        inline GameLogicSystem*    getGameLogicSystem() { return _gameLogicSystem; };
        inline BusStopSystem*      getBusStopSystem() { return _busStopSystem; };


        SceneObject*    addSceneObject(const std::string& name, RObject* objectDefinition = nullptr);
        void            removeSceneObject(SceneObject* object, bool removeChildren = true);
        void            removeChildSceneObject(SceneObject* object, bool removeChildren = true);

        void            removeSceneObject(const std::string& name, bool removeChildren = true);

        void clearScene();

        SceneObject*    getSceneObject(const std::string& name);
        std::list<SceneObject*>& getSceneObjects();

};


#endif // SCENEMANAGER_H_INCLUDED
