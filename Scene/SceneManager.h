#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED


#include <list>
#include <map>

#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Roads.h"
#include "../Physics/PhysicsManager.hpp"
#include "SceneObject.h"
#include "Skybox.h"


class SceneManager
{
    private:
        GraphicsManager*    _graphicsManager;
        PhysicsManager*     _physicsManager;

        std::list<SceneObject*> _sceneObjects;

        Skybox* _sky;

    public:
        SceneManager(PhysicsManager* pMgr);
        ~SceneManager();

        //GraphicsManager*    getGraphicsManager();
        PhysicsManager*     getPhysicsManager() { return _physicsManager; };


        SceneObject*    addSceneObject(std::string name);
        void            removeSceneObject(SceneObject* object);

        void            removeSceneObject(std::string name);

        SceneObject*    getSceneObject(std::string name);

        void addSky(RTextureCubeMap* texture);



        void loadScene(std::string filename);

        void loadObject(std::string name, glm::vec3 position = glm::vec3(0,0,0), glm::vec3 rotation = glm::vec3(0,0,0));
};


#endif // SCENEMANAGER_H_INCLUDED
