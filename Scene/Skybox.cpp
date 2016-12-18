#include "Skybox.h"

#include "SceneManager.h"


Skybox::Skybox(RTextureCubeMap* texture, SceneManager* sceneManager)
    : SceneObject("sky", sceneManager)
{
    //RModel* skyModel = ResourceManager::getInstance().loadModel("data/cube.3ds", "");
    Material skyMaterial;
    skyMaterial.diffuseTexture = texture;
    skyMaterial.shader = SKY_MATERIAL;
    /*for (int i = 0; i < skyModel->_quantumOfMeshes; ++i)
    {
        skyModel->_meshes[i].material = skyMaterial;
    }*/

    Prefab* cube = new Cube(900, skyMaterial);
    cube->init();
    RenderObject* skyRenderObject = GraphicsManager::getInstance().addRenderObject(cube);
    addComponent(skyRenderObject);

    //getTransform()->setScale(glm::vec3(20, 20, 20));
}


Skybox::~Skybox()
{

}
