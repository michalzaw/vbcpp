#include "MenuSelectBusScene.h"

#include "CameraControlComponent.h"
#include "Directories.h"
#include "GameLogicSystem.h"
#include "GameScenesNames.h"
#include "MainGameScene.h"

#include "../Bus/BusPreviewLoader.h"

#include "../Graphics/Renderer.h"

#include "../ImGuiInterface/MenuSelectBusInterfaceWindow.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/InputSystem.h"
#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"


MenuSelectBusScene::MenuSelectBusScene(Window* window)
	: GameScene(window),
	_selectedBus(0), _selectedBusConfigurationIndex(0), _selectedBusRepaintName(""),
	_menuInterfaceWindow(nullptr)
{

}


MenuSelectBusScene::~MenuSelectBusScene()
{

}


void MenuSelectBusScene::loadAvailableBusesNames()
{
	/*
	_availableBusesNames = FilesHelper::getDirectoriesList(GameDirectories::BUSES);
#ifdef DEVELOPMENT_RESOURCES
	std::vector<std::string> availableBusesDev = FilesHelper::getDirectoriesList(GameConfig::getInstance().alternativeResourcesPath + GameDirectories::BUSES);
	_availableBusesNames.insert(_availableBusesNames.end(), availableBusesDev.begin(), availableBusesDev.end());
#endif // DEVELOPMENT_RESOURCES
	*/

	//_availableBusesNames.push_back("MAN");
	_availableBusesNames.push_back("neoplan");
	_availableBusesNames.push_back("Solaris_IV");
	_availableBusesNames.push_back("Solaris_IV_PBR");
	_availableBusesNames.push_back("h9_raycast");
}


CameraFPS* MenuSelectBusScene::createCameraFPSGlobal()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("cameraFPSGlobal");

	CameraFPS* cameraFPS = _graphicsManager->addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(0, degToRad(180.0f), 0);
	cameraObject->setPosition(0, 0.5f, 15);

	return cameraFPS;
}


void MenuSelectBusScene::addBus(const std::string& modelFileName, const std::string& texturePath)
{
	SceneObject* busSceneObject = _sceneManager->addSceneObject("bus");

	RStaticModel* busModel = ResourceManager::getInstance().loadModel(modelFileName, texturePath);
	RenderObject* busRenderObject = _graphicsManager->addRenderObject(new RenderObject(busModel, true), busSceneObject);

	_buses.push_back(busSceneObject);
}


void MenuSelectBusScene::addBus(const std::string& name)
{
	BusPreviewLoader busLoader(_sceneManager, _graphicsManager, _physicsManager, _soundManager);
	
	BusPreview* busPreview = busLoader.loadBusPreview(name);
	busPreview->bus->getSceneObject()->setIsActive(false);

	_buses2.push_back(busPreview);
}


void MenuSelectBusScene::selectNextBus()
{
	/*_buses[_selectedBus]->setIsActive(false);
	_selectedBus = (_selectedBus + 1) % _buses.size();
	_buses[_selectedBus]->setRotation(0.0f, 0.0f, 0.0f);
	_buses[_selectedBus]->setIsActive(true);*/

	_buses2[_selectedBus]->bus->getSceneObject()->setIsActive(false);
	_selectedBus = (_selectedBus + 1) % _buses2.size();

	showSelectedBus();
}


void MenuSelectBusScene::selectPreviousBus()
{
	/*_buses[_selectedBus]->setIsActive(false);
	_selectedBus = (_selectedBus - 1) % _buses.size();
	_buses[_selectedBus]->setRotation(0.0f, 0.0f, 0.0f);
	_buses[_selectedBus]->setIsActive(true);*/

	_buses2[_selectedBus]->bus->getSceneObject()->setIsActive(false);
	_selectedBus = _selectedBus - 1;
	if (_selectedBus < 0)
	{
		_selectedBus = _buses2.size() - 1;
	}

	showSelectedBus();
}


void MenuSelectBusScene::showSelectedBus()
{
	_buses2[_selectedBus]->bus->getSceneObject()->setRotation(0.0f, 0.0f, 0.0f);
	_buses2[_selectedBus]->bus->getSceneObject()->setIsActive(true);

	_selectedBusConfigurationIndex = 0;

	auto firstConfiguration = _buses2[_selectedBus]->predefinedConfigurations.begin();
	if (firstConfiguration != _buses2[_selectedBus]->predefinedConfigurations.end())
	{
		_selectedBusConfigurationVariables = firstConfiguration->second;
	}
	else
	{
		_selectedBusConfigurationVariables.clear();
	}

	_buses2[_selectedBus]->setConfiguration(_selectedBusConfigurationVariables);

	showBusLogo();

	_menuInterfaceWindow->setCurrentBusPreview(_buses2[_selectedBus]);
}


void MenuSelectBusScene::showBusLogo()
{
	const std::string& logoFileName = _buses2[_selectedBus]->bus->getBusDescription().logo;
	if (!logoFileName.empty())
	{
		_busLogo->setTexture(ResourceManager::getInstance().loadTexture(logoFileName));

		float scale = _window->getWidth() / 10.0f / _busLogo->getSize().x;

		_busLogo->setScale(scale, scale);
		_busLogo->setPosition(_window->getWidth() - _busLogo->getRealSize().x - 50.0f, _window->getHeight() - _busLogo->getRealSize().y - 50.0f);
		//_busLogo->setPosition(0.0f, _window->getHeight() - _busLogo->getRealSize().y);
		_busLogo->setIsActive(true);
	}
	else
	{
		_busLogo->setIsActive(false);
	}
}


void MenuSelectBusScene::initialize()
{
	loadAvailableBusesNames();

	CameraFPS* camera = createCameraFPSGlobal();

	_graphicsManager->setCurrentCamera(camera);
	_soundManager->setActiveCamera(camera);

	_busLogo = _gui->addImage(ResourceManager::getInstance().loadDefaultWhiteTexture());
	_busLogo->setIsActive(false);

	Label* description = _gui->addLabel(ResourceManager::getInstance().loadFont("Fonts/arial.ttf"));
	description->setPosition(0, 500);
	description->setMaxWidth(800);
	description->setMaxHeight(600);
	description->setScale(0.4f, 0.4f);
	description->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

	/*RTexture2D* logoTexture = ResourceManager::getInstance().loadTexture("Data/logo.jpg");
	Image* logoImage = _gui->addImage(logoTexture);

	float scale = _window->getWidth() / 10.0f / logoTexture->getSize().x;
	logoImage->setScale(scale, scale);
	logoImage->setPosition(0.0f, _window->getHeight() - logoImage->getRealSize().y);*/

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
	Light* light = _graphicsManager->addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f);
	lightSceneObject->addComponent(light);
	lightSceneObject->setRotation(0, PI, 0);
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

	// buses
	//addBus("Buses/Solaris_IV/neoplan.fbx", "Buses/Solaris_IV/");
	//addBus("Buses\\neoplan\\neoplan.fbx", "Buses\\neoplan\\");

	for (const std::string& busName : _availableBusesNames)
	{
		addBus(busName);
	}

	//_buses[0]->setPosition(0.0f, -1.5f, 0.0f);
	_buses2[2]->bus->getSceneObject()->setPosition(0.0f, -1.5f, 0.0f);
	_buses2[1]->bus->getSceneObject()->setPosition(0.0f, -1.5f, 0.0f);

	_menuInterfaceWindow = new MenuSelectBusInterfaceWindow(&_selectedBusConfigurationVariables, _selectedBusRepaintName, _sceneManager, true);
	_imGuiInterface->addWindow(_menuInterfaceWindow);

	showSelectedBus();
}


void MenuSelectBusScene::fixedStepUpdate(double deltaTime)
{
	//_buses[_selectedBus]->rotate(0.0f, degToRad(10.0f * deltaTime), 0.0f);
	_buses2[_selectedBus]->bus->getSceneObject()->rotate(0.0f, degToRad(10.0f * deltaTime), 0.0f);
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
	if (input.isKeyPressed(GLFW_KEY_ENTER))
	{
		std::unordered_map<std::string, std::string> params;
		MainGameScene::createSceneParams(params, _buses2[_selectedBus]->busName, _selectedBusRepaintName, _selectedBusConfigurationVariables);

		setNextGameScene(GameScenesNames::MAIN_SCENE, true, params);
	}
}


void MenuSelectBusScene::terminate()
{
	for (BusPreview* busPreview : _buses2)
	{
		delete busPreview;
	}
}

