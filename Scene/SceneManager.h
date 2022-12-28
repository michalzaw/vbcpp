#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED


#include <list>
#include <unordered_map>

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
        static const ObjectId MIN_OBJECT_ID = 1u;
        static const ObjectId MAX_OBJECT_ID = 0xffffffff;

        GraphicsManager*    _graphicsManager;
        PhysicsManager*     _physicsManager;
        SoundManager*       _soundManager;
        GameLogicSystem*    _gameLogicSystem;
        BusStopSystem*      _busStopSystem;

        std::list<SceneObject*> _sceneObjects;
        std::unordered_map<ObjectId, SceneObject*> _sceneObjectsMap;

        ObjectId _nextObjectId;

        ObjectId generateNewObjectId();

    public:
        SceneManager(GraphicsManager* gMgr, PhysicsManager* pMgr, SoundManager* sndMgr);
        ~SceneManager();

        inline GraphicsManager*    getGraphicsManager() { return _graphicsManager; };
        inline PhysicsManager*     getPhysicsManager() { return _physicsManager; };
        inline SoundManager*       getSoundManager() { return _soundManager; };
        inline GameLogicSystem*    getGameLogicSystem() { return _gameLogicSystem; };
        inline BusStopSystem*      getBusStopSystem() { return _busStopSystem; };


        SceneObject*    addSceneObject(const std::string& name, ObjectId id = 0u, RObject* objectDefinition = nullptr);
        void            removeSceneObject(SceneObject* object, bool removeChildren = true);
        void            removeChildSceneObject(SceneObject* object, bool removeChildren = true);

        void            removeSceneObject(const std::string& name, bool removeChildren = true);
        void            removeSceneObject(ObjectId id, bool removeChildren = true);

        void clearScene();

        SceneObject*    getSceneObject(const std::string& name);
        SceneObject*    getSceneObject(ObjectId id);
        std::list<SceneObject*>& getSceneObjects();

};


#endif // SCENEMANAGER_H_INCLUDED
