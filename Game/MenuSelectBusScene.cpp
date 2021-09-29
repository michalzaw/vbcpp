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
#include "../Utils/LocalizationSystem.h"
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
		_selectedBusConfigurationVariables = firstConfiguration->configuration;
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


void MenuSelectBusScene::createConfigurationWindow()
{
	RFont* fontBoldItalic32 = ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-BoldItalic.ttf", 32);
	RFont* fontRegular26 = ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 22);
	glm::vec2 pickerMargin = glm::vec2(5.0f, 7.5f);
	//glm::vec4 pickerBackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.1f);
	//glm::vec4 pickerBackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.3f);
	glm::vec4 pickerBackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

	int pickerHeight = 30;
	int pickerYSpace = 5;
	int windowWidth = 440;
	int windowHeight = 90 + (pickerHeight + pickerYSpace) * 4 + 15;

	//glm::vec2 startPosition = glm::vec2(650, 340);
	glm::vec2 startPosition = glm::vec2(_window->getWidth() - windowWidth - 50, windowHeight + 50);

	Image* imageBackground = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(0.0f, 0.0f, 0.0f, 0.2f)));
	imageBackground->setScale(windowWidth / imageBackground->getTexture()->getSize().x, windowHeight / imageBackground->getTexture()->getSize().y);
	imageBackground->setPosition(startPosition.x, startPosition.y - windowHeight);

	Label* labelTitle = _gui->addLabel(fontBoldItalic32, GET_TEXT(CONFIGURATION));
	labelTitle->setPosition(startPosition + glm::vec2(40, -40));
	labelTitle->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	Image* imgLine = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	imgLine->setPosition(startPosition + glm::vec2(20, -50));
	imgLine->setScale(200, 1);

	int y = -90;

	std::vector<std::string> optionsConfigurations;
	for (const auto& predefinedConfiguration : _buses2[_selectedBus]->predefinedConfigurations)
	{
		const std::string& displayName = _buses2[_selectedBus]->bus->getText(predefinedConfiguration.displayName);
		optionsConfigurations.push_back(displayName);
	}
	Picker* picker1 = _gui->addPicker(fontRegular26, optionsConfigurations, 400, pickerHeight);
	picker1->setPosition(startPosition + glm::vec2(20, y - pickerMargin.y));
	picker1->setBackgroundColor(pickerBackgroundColor);
	picker1->setMargin(pickerMargin);
	picker1->setOnValueChangedCallback([this](int index, const std::string& value)
		{
			_selectedBusConfigurationVariables = _buses2[_selectedBus]->predefinedConfigurations[index].configuration;
			_selectedBusConfigurationIndex = index;

			_buses2[_selectedBus]->setConfiguration(_selectedBusConfigurationVariables);
			_buses2[_selectedBus]->bus->getSceneObject()->setRotation(0.0f, degToRad(0.0f), 0.0f);

			setValuesInVariablesPickers();
		});

	for (int i = 0; i < _buses2[_selectedBus]->availableVariables.size(); ++i)
	{
		const GameVariable& variable = _buses2[_selectedBus]->availableVariables[i];

		y -= pickerHeight + pickerYSpace;
		int pickerWidth = 150;

		Label* label = _gui->addLabel(fontRegular26, _buses2[_selectedBus]->bus->getText(variable.displayName));
		label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label->setPosition(startPosition + glm::vec2(20, y));

		std::vector<std::string> optionsVariableValues;
		for (const auto& variableValue : variable.values)
		{
			const std::string& displayValue = _buses2[_selectedBus]->bus->getText(variableValue.displayValue);
			optionsVariableValues.push_back(displayValue);
		}
		Picker* picker = _gui->addPicker(fontRegular26, optionsVariableValues, pickerWidth, pickerHeight);
		picker->setPosition(startPosition + glm::vec2(windowWidth - pickerWidth - 20, y - pickerMargin.y));
		picker->setBackgroundColor(pickerBackgroundColor);
		picker->setMargin(pickerMargin);
		picker->setOnValueChangedCallback([this, variable](int index, const std::string& value)
			{
				_selectedBusConfigurationVariables[variable.name] = variable.values[index].value;

				_buses2[_selectedBus]->setConfiguration(_selectedBusConfigurationVariables);
				_buses2[_selectedBus]->bus->getSceneObject()->setRotation(0.0f, degToRad(0.0f), 0.0f);
			});

		_busConfigurationVariablesPickers.push_back(picker);
	}

	setValuesInVariablesPickers();
}


void MenuSelectBusScene::setValuesInVariablesPickers()
{
	for (int i = 0; i < _buses2[_selectedBus]->availableVariables.size(); ++i)
	{
		const std::string& variableValue = _selectedBusConfigurationVariables[_buses2[_selectedBus]->availableVariables[i].name];

		const auto findResult = std::find_if(
			_buses2[_selectedBus]->availableVariables[i].values.begin(),
			_buses2[_selectedBus]->availableVariables[i].values.end(),
			[variableValue](const VariableValue& v) { return v.value == variableValue; }
		);
		if (findResult != _buses2[_selectedBus]->availableVariables[i].values.end())
		{
			const std::string& displayValue = _buses2[_selectedBus]->bus->getText(findResult->displayValue);
			_busConfigurationVariablesPickers[i]->setSelectedOption(displayValue);
		}
	}
}


void MenuSelectBusScene::createConfigurationPreviewWindow()
{
	{
		Image* imageBackground = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(0.0f, 0.0f, 0.0f, 0.1f)));
		imageBackground->setScale(220, 110);
		imageBackground->setPosition(160, 120);


		RFont* fontBoldItalic32 = ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-BoldItalic.ttf", 32);
		RFont* fontRegular26 = ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 26);
		glm::vec2 pickerMargin = glm::vec2(5.0f, 7.5f);
		//glm::vec4 pickerBackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.1f);
		//glm::vec4 pickerBackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.3f);
		glm::vec4 pickerBackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

		Label* labelTitle = _gui->addLabel(fontBoldItalic32, GET_TEXT(CONFIGURATION));
		labelTitle->setPosition(200, 300);
		labelTitle->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		Image* imgLine = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		imgLine->setPosition(180, 290);
		imgLine->setScale(200, 1);

		std::vector<std::string> optionsConfigurations;
		optionsConfigurations.push_back("Typ 1");
		optionsConfigurations.push_back("Typ 2");
		optionsConfigurations.push_back("Test123");
		Picker* picker1 = _gui->addPicker(fontRegular26, optionsConfigurations, 400, 40);
		picker1->setPosition(180, 250 - pickerMargin.y);
		picker1->setBackgroundColor(pickerBackgroundColor);
		picker1->setMargin(pickerMargin);
		picker1->setOnValueChangedCallback([](int index, const std::string& value)
			{
				LOG_DEBUG("Selected item: " + value + " (" + Strings::toString(index) + ")");
			});

		Label* labelFront = _gui->addLabel(fontRegular26, "Front pojazdu");
		labelFront->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		labelFront->setPosition(180, 200);

		std::vector<std::string> optionsFront;
		optionsFront.push_back("1");
		optionsFront.push_back("2");
		Picker* picker2 = _gui->addPicker(fontRegular26, optionsFront, 200, 40);
		picker2->setPosition(380, 200 - pickerMargin.y);
		picker2->setBackgroundColor(pickerBackgroundColor);
		picker2->setMargin(pickerMargin);

		Label* labelDisplay = _gui->addLabel(fontRegular26, "Wyswietlacz");
		labelDisplay->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		labelDisplay->setPosition(180, 150);

		std::vector<std::string> optionsDisplay;
		optionsDisplay.push_back("true");
		optionsDisplay.push_back("false");
		Picker* picker3 = _gui->addPicker(fontRegular26, optionsDisplay, 200, 40);
		picker3->setPosition(380, 150 - pickerMargin.y);
		picker3->setBackgroundColor(pickerBackgroundColor);
		picker3->setMargin(pickerMargin);
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

	createConfigurationWindow();
	//createConfigurationPreviewWindow();
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

