#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED


#include <list>

#include "../Graphics/GraphicsManager.h"
#include "../Physics/PhysicsManager.hpp"
#include "SceneObject.h"
#include "Skybox.h"


class SceneManager
{
    private:
        GraphicsManager*    _graphicsManager;
        //PhysicsManager*     _physicsManager;

        std::list<SceneObject*> _sceneObjects;

        Skybox* _sky;

    public:
        SceneManager();
        ~SceneManager();

        //GraphicsManager*    getGraphicsManager();
        //PhysicsManager*     getPhysicsManager();


        SceneObject*    addSceneObject(std::string name);
        void            removeSceneObject(SceneObject* object);

        void            removeSceneObject(std::string name);

        SceneObject*    getSceneObject(std::string name);

        void addSky(RTextureCubeMap* texture);

};


#endif // SCENEMANAGER_H_INCLUDED
