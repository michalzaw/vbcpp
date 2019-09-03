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


struct BusStart
{
    glm::vec3 position;
    glm::vec3 rotation;
};

class SceneManager
{
    private:
        GraphicsManager*    _graphicsManager;
        PhysicsManager*     _physicsManager;
        SoundManager*       _soundManager;

        std::list<SceneObject*> _sceneObjects;

        BusStart    _busStart;

    public:
        SceneManager(PhysicsManager* pMgr, SoundManager* sndMgr);
        ~SceneManager();

        //GraphicsManager*    getGraphicsManager();
        PhysicsManager*     getPhysicsManager() { return _physicsManager; };
        SoundManager*       getSoundManager() { return _soundManager; };


        SceneObject*    addSceneObject(std::string name, RObject* objectDefinition = NULL);
        void            removeSceneObject(SceneObject* object, bool removeChildren = true);

        void            removeSceneObject(std::string name, bool removeChildren = true);

        void clearScene()
        {
            for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
            {
                delete *i;
            }

            _sceneObjects.clear();

            GraphicsManager::getInstance().clearQuadTree();
        }

        SceneObject*    getSceneObject(std::string name);
        std::list<SceneObject*>& getSceneObjects();

        BusStart& getBusStart() { return _busStart; }

};


#endif // SCENEMANAGER_H_INCLUDED
