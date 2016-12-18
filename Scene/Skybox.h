#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED


#include "SceneObject.h"

#include "../Graphics/RenderObject.h"
#include "../Graphics/Prefab.h"

#include "../Utils/ResourceManager.h"
#include "../Utils/RTextureCubeMap.h"


class Skybox : public SceneObject
{
    private:

    public:
        Skybox(RTextureCubeMap* texture, SceneManager* sceneManager);
        ~Skybox();

};


#endif // SKYBOX_H_INCLUDED
