#include "MenuSelectBusScene.h"

#include "CameraControlComponent.h"
#include "GameLogicSystem.h"

#include "../Bus/BusLoader.h"

#include "../Graphics/Renderer.h"

#include "../Utils/InputSystem.h"
#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"


MenuSelectBusScene::MenuSelectBusScene(Window* window, PhysicsManager* physicsManager, SoundManager* soundManager, SceneManager* sceneManager, GUIManager* gui)
	: GameScene(window, physicsManager, soundManager, sceneManager, gui),
	_selectedBus(0)
{

}


MenuSelectBusScene::~MenuSelectBusScene()
{

}


CameraFPS* MenuSelectBusScene::createCameraFPSGlobal()
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

	return cameraFPS;
}


void MenuSelectBusScene::addBus(const std::string& modelFileName, const std::string& texturePath)
{
	SceneObject* busSceneObject = _sceneManager->addSceneObject("bus");

	RStaticModel* busModel = ResourceManager::getInstance().loadModel(modelFileName, texturePath);
	RenderObject* busRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel, true), busSceneObject);

	_buses.push_back(busSceneObject);
}


void MenuSelectBusScene::addBus(const std::string& name)
{
	BusLoader busLoader(_sceneManager, _physicsManager, _soundManager);
	
	Bus* bus = busLoader.loadBus(name);
	bus->getSceneObject()->setIsActive(false);

	_buses2.push_back(bus);
}


void MenuSelectBusScene::selectNextBus()
{
	/*_buses[_selectedBus]->setIsActive(false);
	_selectedBus = (_selectedBus + 1) % _buses.size();
	_buses[_selectedBus]->setRotation(0.0f, 0.0f, 0.0f);
	_buses[_selectedBus]->setIsActive(true);*/

	_buses2[_selectedBus]->getSceneObject()->setIsActive(false);
	_selectedBus = (_selectedBus + 1) % _buses2.size();
	_buses2[_selectedBus]->getSceneObject()->setRotation(0.0f, 0.0f, 0.0f);
	_buses2[_selectedBus]->getSceneObject()->setIsActive(true);
}


void MenuSelectBusScene::selectPreviousBus()
{
	/*_buses[_selectedBus]->setIsActive(false);
	_selectedBus = (_selectedBus - 1) % _buses.size();
	_buses[_selectedBus]->setRotation(0.0f, 0.0f, 0.0f);
	_buses[_selectedBus]->setIsActive(true);*/

	_buses2[_selectedBus]->getSceneObject()->setIsActive(false);
	_selectedBus = _selectedBus - 1;
	if (_selectedBus < 0)
	{
		_selectedBus = _buses2.size() - 1;
	}
	_buses2[_selectedBus]->getSceneObject()->setRotation(0.0f, 0.0f, 0.0f);
	_buses2[_selectedBus]->getSceneObject()->setIsActive(true);
}


void MenuSelectBusScene::initialize()
{
	_physicsManager->stop();

	CameraFPS* camera = createCameraFPSGlobal();

	GraphicsManager::getInstance().setCurrentCamera(camera);
	_soundManager->setActiveCamera(camera);

	/*RTexture2D* logoTexture = ResourceManager::getInstance().loadTexture("Data/logo.jpg");
	Image* logoImage = _gui->addImage(logoTexture);

	float scale = _window->getWidth() / 2.0f / logoTexture->getSize().x;
	logoImage->setScale(scale, scale);
	logoImage->setPosition(_window->getWidth() / 2.0f - logoImage->getRealSize().x / 2.0f, _window->getHeight() / 2.0f - logoImage->getRealSize().y / 2.0f);*/

	/*RTexture2D* arrow1Texture = ResourceManager::getInstance().loadTexture("Data/arrow_back.png");
	Button* previousBusButton = _gui->addButton(arrow1Texture);
	previousBusButton->setScale(2, 2);
	previousBusButton->setPosition(0.0f, _window->getHeight() / 2.0f - previousBusButton->getRealSize().y / 2.0f);
	previousBusButton->setOnClickCallback(std::bind(&MenuSelectBusScene::selectPreviousBus, this));

	RTexture2D* arrow2Texture = ResourceManager::getInstance().loadTexture("Data/arrow_forward.png");
	Button* nextBusButton = _gui->addButton(arrow2Texture);
	nextBusButton->setScale(2.0f, 2.0f);
	nextBusButton->setPosition(_window->getWidth() - nextBusButton->getRealSize().x, _window->getHeight() / 2.0f - nextBusButton->getRealSize().y / 2.0f);
	nextBusButton->setOnClickCallback(std::bind(&MenuSelectBusScene::selectNextBus, this));*/

	// light
	SceneObject* lightSceneObject = _sceneManager->addSceneObject("bus");
	Light* light = GraphicsManager::getInstance().addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f);
	lightSceneObject->addComponent(light);
	light->setShadowMapping(GameConfig::getInstance().isShadowmappingEnable);

	// sky
	std::vector<std::string> skyboxFileNamesArray;
	skyboxFileNamesArray.push_back("Skybox/skybox_negx.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_posx.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_negy.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_posy.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_negz.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_posz.hdr");

	RTextureCubeMap* skyboxTexture = ResourceManager::getInstance().loadTextureCubeMap(&skyboxFileNamesArray[0]);
	GraphicsManager::getInstance().addGlobalEnvironmentCaptureComponent(skyboxTexture);

	// buses
	//addBus("Buses/Solaris_IV/neoplan.fbx", "Buses/Solaris_IV/");
	//addBus("Buses\\neoplan\\neoplan.fbx", "Buses\\neoplan\\");

	addBus("MAN");
	addBus("neoplan");
	addBus("Solaris_IV");
	addBus("Solaris_IV_PBR");
	addBus("h9_raycast");

	_buses2[0]->getSceneObject()->setIsActive(true);

	//_buses[0]->setPosition(0.0f, -1.5f, 0.0f);
	_buses2[2]->getSceneObject()->setPosition(0.0f, -1.5f, 0.0f);
	_buses2[3]->getSceneObject()->setPosition(0.0f, -1.5f, 0.0f);

	Renderer::getInstance().bakeStaticShadows();
}


void MenuSelectBusScene::fixedStepUpdate(double deltaTime)
{
	//_buses[_selectedBus]->rotate(0.0f, degToRad(10.0f * deltaTime), 0.0f);
	_buses2[_selectedBus]->getSceneObject()->rotate(0.0f, degToRad(10.0f * deltaTime), 0.0f);
}


void MenuSelectBusScene::update(double deltaTime)
{

}


void MenuSelectBusScene::readInput(double deltaTime)
{

}


void MenuSelectBusScene::fixedStepReadInput(float deltaTime)
{
	InputSystem& input = InputSystem::getInstance();

	if (input.isKeyPressed(GLFW_KEY_LEFT))
	{
		selectPreviousBus();
	}
	else if (input.isKeyPressed(GLFW_KEY_RIGHT))
	{
		selectNextBus();
	}
}


void MenuSelectBusScene::terminate()
{

}

