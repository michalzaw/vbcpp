#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED


#include <list>

#include "../Graphics/GraphicsManager.h"
#include "../Physics/PhysicsManager.hpp"
#include "SceneObject.h"


class SceneManager
{
    private:
        GraphicsManager*    _graphicsManager;
        //PhysicsManager*     _physicsManager;

        std::list<SceneObject*> _sceneObjects;

    public:
        SceneManager();
        ~SceneManager();

        GraphicsManager*    getGraphicsManager();
        //PhysicsManager*     getPhysicsManager();

        SceneObject*    addSceneObject();
        void            removeSceneObject(SceneObject* object);

};


#endif // SCENEMANAGER_H_INCLUDED
