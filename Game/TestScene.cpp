#include "TestScene.h"

#include "CameraControlComponent.h"
#include "Directories.h"
#include "GameLogicSystem.h"

#include "../Bus/BusLoader.h"

#include "../Graphics/Renderer.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/InputSystem.h"
#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"


TestScene::TestScene(Window* window)
	: GameScene(window)
{

}


TestScene::~TestScene()
{

}


CameraFPS* TestScene::createCameraFPSGlobal()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("cameraFPSGlobal");

	CameraFPS* cameraFPS = _graphicsManager->addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(0, 0, 0);
	cameraObject->setPosition(0, 0.5f, -15);

	CameraControlComponent* cameraControlComponent = _sceneManager->getGameLogicSystem()->addCameraControlComponent(cameraFPS);
	cameraObject->addComponent(cameraControlComponent);

	cameraControlComponent->setIsActive(true);

	return cameraFPS;
}


void TestScene::initialize()
{
	CameraFPS* camera = createCameraFPSGlobal();

	_graphicsManager->setCurrentCamera(camera);
	_soundManager->setActiveCamera(camera);

	// light
	SceneObject* lightSceneObject = _sceneManager->addSceneObject("bus");
	Light* light = _graphicsManager->addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f);
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
	_graphicsManager->addGlobalEnvironmentCaptureComponent(skyboxTexture);

	Transform transform;
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchy("Buses/neoplan/neoplan2.fbx", "Buses/neoplan");
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchyOnlyNode("Buses/neoplan/neoplan.fbx", "Buses/neoplan", "tuer0E", transform);
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchyOnlyNode("Buses/neoplan/neoplan.fbx", "Buses/neoplan", "wagenkasten_main", transform);
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchy("Buses/MAN/pulpit_new2.fbx", "Buses/MAN/Konin/");


	//SceneObject* busObject = _sceneManager->addSceneObject("bus");
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchyOnlyNode("Buses/neoplan/neoplan.fbx", "Buses/neoplan", "tuer0E", transform);
	//_graphicsManager->addRenderObject(new RenderObject(busModel, busModel->getNodeByName("tuer0E")), busObject);

	/*RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchy("Buses/neoplan/neoplan.fbx", "Buses/neoplan");


	busObject = _sceneManager->addSceneObject("bus");

	std::vector<std::string> nodesToSkip = { "tuer0E", "tuer1E", "tuer2E", "tuer3E", "tuer4E", "tuer5E", "wagenkasten_front_N40xx", "wagenkasten_front_N4xx" };
	_graphicsManager->addRenderObject(new RenderObject(busModel, nodesToSkip), busObject);

	SceneObject* doorObject = _sceneManager->addSceneObject("door");
	StaticModelNode* modelNode = busModel->getNodeByName("tuer0E");
	_graphicsManager->addRenderObject(new RenderObject(busModel, modelNode), doorObject);
	doorObject->setPosition(modelNode->transform.getPosition());
	doorObject->setRotation(modelNode->transform.getRotation());
	doorObject->setScale(modelNode->transform.getScale());

	busObject->addChild(doorObject);

	SceneObject* frontObject = _sceneManager->addSceneObject("front");
	StaticModelNode* front1lNode = busModel->getNodeByName("wagenkasten_front_N40xx");
	StaticModelNode* front2lNode = busModel->getNodeByName("wagenkasten_front_N4xx");
	_graphicsManager->addRenderObject(new RenderObject(busModel, front2lNode), frontObject);
	frontObject->setPosition(front1lNode->transform.getPosition());
	frontObject->setRotation(front1lNode->transform.getRotation());
	frontObject->setScale(front1lNode->transform.getScale());

	busObject->addChild(frontObject);*/

	BusLoader busLoader(_sceneManager, _graphicsManager, _physicsManager, _soundManager);
	Bus* bus = busLoader.loadBus("neoplan");
	bus->getSceneObject()->move(1.0f, 0.0f, 0.0f);
}


void TestScene::fixedStepUpdate(double deltaTime)
{
	//busObject->rotate(0.0f, degToRad(10.0f * deltaTime), 0.0f);

	_sceneManager->getGameLogicSystem()->update(deltaTime);
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

