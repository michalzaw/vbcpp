#include "TestScene.h"

#include "CameraControlComponent.h"
#include "Directories.h"
#include "GameLogicSystem.h"

#include "../Graphics/Renderer.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/InputSystem.h"
#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"


TestScene::TestScene(Window* window, PhysicsManager* physicsManager, SoundManager* soundManager, SceneManager* sceneManager, GUIManager* gui)
	: GameScene(window, physicsManager, soundManager, sceneManager, gui)
{

}


TestScene::~TestScene()
{

}


CameraFPS* TestScene::createCameraFPSGlobal()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("cameraFPSGlobal");

	CameraFPS* cameraFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(0, 0, 0);
	cameraObject->setPosition(0, 0.5f, -15);

	CameraControlComponent* cameraControlComponent = GameLogicSystem::getInstance().addCameraControlComponent(cameraFPS);
	cameraObject->addComponent(cameraControlComponent);

	cameraControlComponent->setIsActive(true);

	return cameraFPS;
}


void TestScene::initialize()
{
	CameraFPS* camera = createCameraFPSGlobal();

	GraphicsManager::getInstance().setCurrentCamera(camera);
	_soundManager->setActiveCamera(camera);

	// light
	SceneObject* lightSceneObject = _sceneManager->addSceneObject("bus");
	Light* light = GraphicsManager::getInstance().addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f);
	lightSceneObject->addComponent(light);
	light->setShadowMapping(GameConfig::getInstance().isShadowmappingEnable);

	// sky
	std::vector<std::string> skyboxFileNamesArray;
	skyboxFileNamesArray.push_back("Skybox/skybox_posx.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_negx.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_posy.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_negy.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_posz.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_negz.hdr");

	RTextureCubeMap* skyboxTexture = ResourceManager::getInstance().loadTextureCubeMap(&skyboxFileNamesArray[0]);
	GraphicsManager::getInstance().addGlobalEnvironmentCaptureComponent(skyboxTexture);

	Transform transform;
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchy("Buses/neoplan/neoplan2.fbx", "Buses/neoplan");
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchyOnlyNode("Buses/neoplan/neoplan.fbx", "Buses/neoplan", "tuer0E", transform);
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchyOnlyNode("Buses/neoplan/neoplan.fbx", "Buses/neoplan", "wagenkasten_main", transform);
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchy("Buses/MAN/pulpit_new2.fbx", "Buses/MAN/Konin/");


	//SceneObject* busObject = _sceneManager->addSceneObject("bus");
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchyOnlyNode("Buses/neoplan/neoplan.fbx", "Buses/neoplan", "tuer0E", transform);
	//GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel, busModel->getNodeByName("tuer0E")), busObject);

	RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchy("Buses/neoplan/neoplan.fbx", "Buses/neoplan");


	busObject = _sceneManager->addSceneObject("bus");

	std::vector<std::string> nodesToSkip = { "tuer0E", "tuer1E", "tuer2E", "tuer3E", "tuer4E", "tuer5E" };
	GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel, nodesToSkip), busObject);

	SceneObject* doorObject = _sceneManager->addSceneObject("bus");
	StaticModelNode* modelNode = busModel->getNodeByName("tuer0E");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel, modelNode), doorObject);
	doorObject->setPosition(modelNode->transform.getPosition());
	doorObject->setRotation(modelNode->transform.getRotation());
	doorObject->setScale(modelNode->transform.getScale());

	busObject->addChild(doorObject);

	Renderer::getInstance().bakeStaticShadows();
}


void TestScene::fixedStepUpdate(double deltaTime)
{
	busObject->rotate(0.0f, degToRad(10.0f * deltaTime), 0.0f);

	GameLogicSystem::getInstance().update(deltaTime);
}


void TestScene::update(double deltaTime)
{

}


void TestScene::readInput(double deltaTime)
{

}


void TestScene::fixedStepReadInput(float deltaTime)
{

}


void TestScene::terminate()
{

}

