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

	cameraControlComponent->setIsActive(false);

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

	/*BusLoader busLoader(_sceneManager, _graphicsManager, _physicsManager, _soundManager);
	Bus* bus = busLoader.loadBus("neoplan");
	bus->getSceneObject()->move(1.0f, 0.0f, 0.0f);*/


	{
		Label* labelTitle = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Italic.ttf", 64), "START");
		labelTitle->setPosition(500, 500);
		labelTitle->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		/*Button* img = _gui->addButton(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			nullptr,
			ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Italic.ttf", 64),
			"START");
		img->setPosition(500, 500);
		img->_image->setScale(100, 50);
		img->_label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		img->_image->setColor(glm::vec4(0.0, 0.0f, 1.0f, 0.0f));*/

		Image* imgLine = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		imgLine->setPosition(480, 490);
		imgLine->setScale(150, 1);
	}


	glm::vec2 startPosition(700, 300);
	{
		Label* labelTitle = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-BoldItalic.ttf", 32), "KONFIGURACJA");
		labelTitle->setPosition(700, 300);
		labelTitle->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		/*Button* img = _gui->addButton(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			nullptr,
			ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-BoldItalic.ttf", 32),
			"KONFIGURACJA");
		img->setPosition(700, 300);
		img->_image->setScale(100, 50);
		img->_label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		img->_image->setColor(glm::vec4(0.0, 0.0f, 1.0f, 0.0f));*/

		Image* imgLine = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		imgLine->setPosition(680, 290);
		imgLine->setScale(200, 1);
	}

	{
		Label* label = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "Typ 1");
		label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label->setScale(0.8, 0.8);
		label->setPosition(680 + (400 - label->getWidth()) / 2.0f, 250);

		Button* b1 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_back2.png"));
		b1->setPosition(680, 250);

		Button* b2 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_forward2.png"));
		b2->setPosition(680 + 400 - b2->getRealSize().x, 250);
	}

	{
		Label* label = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "Front pojazdu");
		label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label->setPosition(680, 200);
		label->setScale(0.8, 0.8);

		Label* label2 = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "1");
		label2->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label2->setPosition(680 + label->getWidth() + 50 + (400 - label->getWidth() - 50 - label2->getWidth()) / 2.0f, 200);
		label2->setScale(0.8, 0.8);

		Button* b1 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_back2.png"));
		b1->setPosition(680 + label->getWidth() + 50, 200);

		Button* b2 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_forward2.png"));
		b2->setPosition(680 + 400 - b2->getRealSize().x, 200);
	}

	{
		Label* label = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "Wyswietlacz");
		label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label->setPosition(680, 150);
		label->setScale(0.8, 0.8);

		Label* label2 = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "true");
		label2->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label2->setPosition(680 + label->getWidth() + 50 + (400 - label->getWidth() - 50 - label2->getWidth()) / 2.0f, 150);
		label2->setScale(0.8, 0.8);

		Button* b1 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_back2.png"));
		b1->setPosition(680 + label->getWidth() + 50, 150);

		Button* b2 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_forward2.png"));
		b2->setPosition(680 + 400 - b2->getRealSize().x, 150);
	}



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

		Label* labelTitle = _gui->addLabel(fontBoldItalic32, "KONFIGURACJA");
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


	std::vector<std::string> options;
	options.push_back("1");
	options.push_back("2");
	options.push_back("3");
	options.push_back("4");
	options.push_back("5");
	options.push_back("6");
	options.push_back("7");
	options.push_back("8");
	options.push_back("9");
	options.push_back("10");
	options.push_back("20");
	Picker* picker1 = _gui->addPicker(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), options, 400, 40);
	picker1->setPosition(100, 50);

	/*float y = _window->getHeight() - 50;
	for (int i = 0; i < 20; ++i)
	{
		int size = 12 + 2 * i;
		y -= size;

		Label* label = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", size), "Font: " + toString(size));
		label->setPosition(10, y);
	}*/
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

