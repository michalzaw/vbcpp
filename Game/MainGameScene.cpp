#include "MainGameScene.h"

#include "CameraControlComponent.h"
#include "GameLogicSystem.h"
#include "Hud.h"

#include "../Bus/BusLoader.h"
#include "../Bus/BusConfigurationsLoader.h"
#include "../Bus/BusRepaintLoader.h"

#include "../ImGuiInterface/BusLineAndDirectionWindow.h"
#include "../ImGuiInterface/ColorsWindow.h"
#include "../ImGuiInterface/PhysicsDebuggerWindow.h"
#include "../ImGuiInterface/VariablesWindow.h"
#include "../ImGuiInterface/MenuBar.h"

#include "../Graphics/Renderer.h"

#include "../Scene/SceneLoader.h"

#include "../Utils/InputSystem.h"
#include "../Utils/ResourceManager.h"
#include "../Utils/Logger.h"
#include "../Utils/RaycastingUtils.h"


enum GameCamera
{
	GC_DRIVER,
	GC_BUS,
	GC_GLOBAL
};


MainGameScene::MainGameScene(Window* window, PhysicsManager* physicsManager, SoundManager* soundManager, SceneManager* sceneManager, GUIManager* gui, ImGuiInterface* imGuiInterface)
	: GameScene(window, physicsManager, soundManager, sceneManager, gui, imGuiInterface),
	_state(GS_LOADING),
	_activeBus(nullptr),
	_activeCamera(nullptr),
	_hud(nullptr),
	_isCameraControll(false), _isMirrorControll(false), _mirrorControllIndex(-1)
{

}


MainGameScene::~MainGameScene()
{

	delete _hud;
}


CameraFPS* MainGameScene::createCameraBusDriver()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("cameraBusDriver");

	CameraFPS* cameraFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(0, 0, 0);
	cameraObject->setPosition(0, 0, 0);

	CameraControlComponent* cameraControlComponent = GameLogicSystem::getInstance().addCameraControlComponent(cameraFPS);
	cameraObject->addComponent(cameraControlComponent);
	cameraControlComponent->setMovmentControl(false);

	return cameraFPS;
}


CameraFPS* MainGameScene::createCameraBus()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("cameraBus");

	CameraFPS* cameraFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(degToRad(0.0f), degToRad(165.0f), degToRad(0.0f));
	cameraObject->setPosition(0, 0, 0);

	cameraFPS->setMaxVerticalAngle(0.0f);

	cameraFPS->setMinPositionOffset(1.0f);
	cameraFPS->setMaxPositionOffset(30.0f);
	cameraFPS->setPositionOffset(10.0f);

	CameraControlComponent* cameraControlComponent = GameLogicSystem::getInstance().addCameraControlComponent(cameraFPS);
	cameraObject->addComponent(cameraControlComponent);
	cameraControlComponent->setMovmentControl(false);

	return cameraFPS;
}


CameraFPS* MainGameScene::createCameraFPSGlobal()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("cameraFPSGlobal");

	CameraFPS* cameraFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(0, 0, 0);
	cameraObject->setPosition(0, 0, 0);

	CameraControlComponent* cameraControlComponent = GameLogicSystem::getInstance().addCameraControlComponent(cameraFPS);
	cameraObject->addComponent(cameraControlComponent);

	return cameraFPS;
}


void MainGameScene::initScene()
{
	_cameras.resize(3);

	_cameras[GC_DRIVER] = createCameraBusDriver();
	_cameras[GC_BUS] = createCameraBus();
	_cameras[GC_GLOBAL] = createCameraFPSGlobal();

	setActiveCamera(_cameras[GC_BUS]);
}


void MainGameScene::setActiveCamera(CameraFPS* camera)
{
	if (_activeCamera != nullptr)
	{
		_activeCamera->getSceneObject()->getComponent(CT_CAMERA_CONTROL)->setIsActive(false);
	}
	camera->getSceneObject()->getComponent(CT_CAMERA_CONTROL)->setIsActive(true);

	_activeCamera = camera;

	GraphicsManager::getInstance().setCurrentCamera(camera);
	_soundManager->setActiveCamera(camera);
}


void MainGameScene::loadScene()
{
	std::unordered_map<std::string, std::string> busVariables;

	BusLoader busLoader(_sceneManager, _physicsManager, _soundManager);

	BusConfigurationsLoader::loadBusPredefinedConfigurationByName(GameConfig::getInstance().busModel, GameConfig::getInstance().busConfiguration, busVariables);
	Bus* bus = busLoader.loadBus(GameConfig::getInstance().busModel, busVariables);
	GameLogicSystem::getInstance().addBus(bus);

	BusConfigurationsLoader::loadBusPredefinedConfigurationByName(GameConfig::getInstance().busModel, "Typ 2", busVariables);
	Bus* bus2 = busLoader.loadBus(GameConfig::getInstance().busModel, busVariables);
	GameLogicSystem::getInstance().addBus(bus2);

	_activeBus = bus;

	SceneLoader sceneLoader(_sceneManager);
	sceneLoader.loadMap(GameConfig::getInstance().mapFile);

	bus->getSceneObject()->setPosition(_sceneManager->getBusStart().position);
	bus->getSceneObject()->setRotation(degToRad(_sceneManager->getBusStart().rotation.x),
		degToRad(_sceneManager->getBusStart().rotation.y),
		degToRad(_sceneManager->getBusStart().rotation.z));

	bus2->getSceneObject()->setPosition(_sceneManager->getBusStart().position + vec3(20.0f, 0.0f, 0.0f));
	bus2->getSceneObject()->setRotation(degToRad(_sceneManager->getBusStart().rotation.x),
		degToRad(_sceneManager->getBusStart().rotation.y),
		degToRad(_sceneManager->getBusStart().rotation.z));

	bus->getSceneObject()->addChild(_cameras[GC_DRIVER]->getSceneObject());
	_cameras[GC_DRIVER]->getSceneObject()->setPosition(_activeBus->getDriverPosition());
	_cameras[GC_DRIVER]->getSceneObject()->setRotation(0, 0, 0);

	bus->getSceneObject()->addChild(_cameras[GC_BUS]->getSceneObject());

	/*CameraStatic* camera = GraphicsManager::getInstance().getCurrentCamera();
	camera->getSceneObject()->setPosition(_sceneManager->getBusStart().position + glm::vec3(-8.0f, -3.0f, -3.0f));
	camera->getSceneObject()->setRotation(degToRad(-5.0f),
										  degToRad(60.0f),
										  degToRad(0.0f));*/


	if (!GameConfig::getInstance().busRepaint.empty())
	{
		std::vector<RMaterialsCollection*> altMaterialsCollections;
		BusRepaintLoader::loadBusRepaint(GameConfig::getInstance().busModel, GameConfig::getInstance().busRepaint, altMaterialsCollections);
		for (RMaterialsCollection* materialsCollection : altMaterialsCollections)
		{
			_activeBus->replaceMaterialsByName(materialsCollection->getMaterials());
		}
	}

	std::vector<RMaterialsCollection*> altMaterialsCollections2;
	BusRepaintLoader::loadBusRepaint(GameConfig::getInstance().busModel, "MPK", altMaterialsCollections2);
	for (RMaterialsCollection* materialsCollection : altMaterialsCollections2)
	{
		bus2->replaceMaterialsByName(materialsCollection->getMaterials());
	}


	Renderer::getInstance().bakeStaticShadows();
}


void MainGameScene::initGui()
{
	_hud = new Hud(_gui, _activeBus);
}


void MainGameScene::initImGuiInterface()
{
	// windows
	ImGuiWindow* busLineAndDirectionWindow = new BusLineAndDirectionWindow(_sceneManager);

	_imGuiInterface->addWindow(busLineAndDirectionWindow);

	if (GameConfig::getInstance().developmentMode)
	{
		ImGuiWindow* colorsWindow = new ColorsWindow(_sceneManager);
		ImGuiWindow* physicsDebuggerWindow = new PhysicsDebuggerWindow(_sceneManager, false);
		ImGuiWindow* variablesWindow = new VariablesWindow(_sceneManager, false);

		_imGuiInterface->addWindow(colorsWindow);
		_imGuiInterface->addWindow(physicsDebuggerWindow);
		_imGuiInterface->addWindow(variablesWindow);

		// menu
		std::vector<MenuItem> windowMenuItems;
		windowMenuItems.push_back(MenuItem("Bus line and direction", busLineAndDirectionWindow->getOpenFlagPointer()));
		windowMenuItems.push_back(MenuItem("Colors", colorsWindow->getOpenFlagPointer()));
		windowMenuItems.push_back(MenuItem("Physics debugger", physicsDebuggerWindow->getOpenFlagPointer()));
		windowMenuItems.push_back(MenuItem("Game variables", variablesWindow->getOpenFlagPointer()));

		std::vector<MenuItem> menuItems;
		menuItems.push_back(MenuItem("Window", windowMenuItems));

		ImGuiWindow* menuBar = new MenuBar(_sceneManager, menuItems);
		_imGuiInterface->addWindow(menuBar);
	}
}


void MainGameScene::startGame()
{
	_state = GS_GAME;

	_physicsManager->play();
	_soundManager->setMute(false);

	glfwSetCursorPos(_window->getWindow(), _window->getWidth() / 2, _window->getHeight() / 2);
}


void MainGameScene::initialize()
{
	initScene();

	loadScene();

	initGui();

	initImGuiInterface();

	startGame();
}


void MainGameScene::fixedStepUpdate(double deltaTime)
{
	_activeBus->update(deltaTime);

	BusStopSystem::getInstance().update(deltaTime, _activeBus);

	if (_isCameraControll)
	{
		GameLogicSystem::getInstance().update(deltaTime);
	}
}


void MainGameScene::update(double deltaTime)
{
	_hud->update(deltaTime);
}


void MainGameScene::readInput(double deltaTime)
{
	GLFWwindow* window = _window->getWindow();

	if (_isMirrorControll)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			_activeBus->getMirror(_mirrorControllIndex)->getSceneObject()->rotate(0.0f, -0.1f * deltaTime, 0.0f);
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			_activeBus->getMirror(_mirrorControllIndex)->getSceneObject()->rotate(0.0f, 0.1f * deltaTime, 0.0f);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			_activeBus->getMirror(_mirrorControllIndex)->getSceneObject()->rotate(0.1f * deltaTime, 0.0f, 0.0f);
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			_activeBus->getMirror(_mirrorControllIndex)->getSceneObject()->rotate(-0.1f * deltaTime, 0.0f, 0.0f);
		}
	}
	else
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			_activeBus->turnLeft(deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			_activeBus->turnRight(deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			_activeBus->accelerate();
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
		{
			_activeBus->idle();
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			_activeBus->brakeOn();
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		{
			_activeBus->brakeOff();
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
		{
			_activeBus->centerSteringWheel(deltaTime);
		}
	}

	static float x = 0;
	static float angle = 0;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if (x < 1.0f)
		{
			x += deltaTime;
			float x1 = glm::mix(-PI / 2.0f, PI / 2.0f, x);
			float sinx1 = sinf(x1);
			sinx1 = sinx1 * 0.5f + 0.5f;
			sinx1 *= 25.0f;

			float delta = sinx1 - angle;
			angle += delta;
			_activeCamera->getSceneObject()->rotate(0, degToRad(-delta), 0);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
	{
		if (x > 0.0f)
		{
			x -= deltaTime;
			float x1 = glm::mix(-PI / 2.0f, PI / 2.0f, x);
			float sinx1 = sinf(x1);
			sinx1 = sinx1 * 0.5f + 0.5f;
			sinx1 *= 25.0f;

			float delta = sinx1 - angle;
			angle += delta;
			_activeCamera->getSceneObject()->rotate(0, degToRad(-delta), 0);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && GameConfig::getInstance().developmentMode)
	{
		Renderer::getInstance().setExposure(Renderer::getInstance().getExposure() * 1.1f);
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && GameConfig::getInstance().developmentMode)
	{
		Renderer::getInstance().setExposure(Renderer::getInstance().getExposure() / 1.1f);
	}
}


void MainGameScene::fixedStepReadInput(float deltaTime)
{
	InputSystem& input = InputSystem::getInstance();

	if (input.isKeyPressed(GLFW_KEY_1))
	{
		_activeBus = GameLogicSystem::getInstance().getBus(0);
	}
	if (input.isKeyPressed(GLFW_KEY_2))
	{
		_activeBus = GameLogicSystem::getInstance().getBus(1);
	}

	if (input.isKeyPressed(GLFW_KEY_L))
	{
		_activeBus->setIsEnableHeadlights(!_activeBus->isEnableHeadlights());
	}
	if (input.isKeyPressed(GLFW_KEY_K))
	{
		_activeBus->setIsEnableLights(!_activeBus->isEnableLights());
	}
	if (input.isKeyPressed(GLFW_KEY_H))
	{
		SceneObject* dirLight = _sceneManager->getSceneObject("sun");
		Light* l = dynamic_cast<Light*>(dirLight->getComponent(CT_LIGHT));

		if (l->getDiffiseIntenisty() > 0.05)
		{
			l->setAmbientIntensity(0.0025);
			l->setDiffuseIntensity(0.0);
			Renderer::getInstance().setDayNightRatio(-1.0f);
		}
		else
		{
			l->setAmbientIntensity(0.025);
			l->setDiffuseIntensity(0.5);
			Renderer::getInstance().setDayNightRatio(1.0f);
		}
	}

	if (input.isKeyPressed(GLFW_KEY_Z))
	{
		_activeBus->doorGroupOpenClose(1);
	}

	if (input.isKeyPressed(GLFW_KEY_X))
	{
		_activeBus->doorGroupOpenClose(2);
	}

	if (input.isKeyPressed(GLFW_KEY_C))
	{
		_activeBus->doorGroupOpenClose(3);
	}

	if (input.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		_activeBus->getGearbox()->shiftUp();
	}
	if (input.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
	{
		_activeBus->getGearbox()->shiftDown();
	}
	if (input.isKeyPressed(GLFW_KEY_0))
	{
		if (!_activeBus->getEngine()->isRunning())
		{
			_activeBus->startEngine();
		}
		else
		{
			_activeBus->stopEngine();
		}
	}

	if (input.isKeyPressed(GLFW_KEY_P))
	{
		if (!_physicsManager->isRunning())
			_physicsManager->play();
		else
			_physicsManager->stop();
	}

	if (input.isKeyPressed(GLFW_KEY_SPACE))
	{
		_activeBus->toggleHandbrake();
	}

	if (input.isKeyPressed(GLFW_KEY_M))
	{
		++_mirrorControllIndex;
		if (_mirrorControllIndex < _activeBus->getMirrorsCount())
		{
			_isMirrorControll = true;
		}
		else
		{
			_isMirrorControll = false;
			_mirrorControllIndex = -1;
		}
	}
	if (input.isKeyPressed(GLFW_KEY_R))
	{
		_activeBus->getSceneObject()->setPosition(_sceneManager->getBusStart().position);
		_activeBus->getSceneObject()->setRotation(degToRad(_sceneManager->getBusStart().rotation.x),
			degToRad(_sceneManager->getBusStart().rotation.y),
			degToRad(_sceneManager->getBusStart().rotation.z));
	}
	if (input.isKeyPressed(GLFW_KEY_F5))
	{
		_cameras[GC_DRIVER]->getSceneObject()->setRotation(0.0f, 0.0f, 0.0f);

		setActiveCamera(_cameras[GC_DRIVER]);
	}
	if (input.isKeyPressed(GLFW_KEY_F6))
	{
		_cameras[GC_BUS]->getSceneObject()->setRotation(0.0f, 0.0f, 0.0f);

		setActiveCamera(_cameras[GC_BUS]);
	}
	if (input.isKeyPressed(GLFW_KEY_F7))
	{
		_cameras[GC_GLOBAL]->getSceneObject()->setPosition(_activeCamera->getPosition());

		setActiveCamera(_cameras[GC_GLOBAL]);
	}

	// debug
	if (GameConfig::getInstance().developmentMode)
	{
		if (input.isKeyPressed(GLFW_KEY_1) && input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			ResourceManager::getInstance().reloadAllShaders();
		}
		if (input.isKeyPressed(GLFW_KEY_2) && input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			ResourceManager::getInstance().reloadAllTextures();
		}
		if (input.isKeyPressed(GLFW_KEY_3) && input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			Renderer::getInstance().toogleRenderAABBFlag();
		}
		if (input.isKeyPressed(GLFW_KEY_4) && input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			Renderer::getInstance().toogleRenderOBBFlag();
		}
		if (input.isKeyPressed(GLFW_KEY_5) && input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			Renderer::getInstance().setAlphaToCoverage(!(Renderer::getInstance().isAlphaToCoverageEnable()));
		}
		if (input.isKeyPressed(GLFW_KEY_8) && input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			Renderer::getInstance().setBloom(!(Renderer::getInstance().isBloomEnable()));
		}
		if (input.isKeyPressed(GLFW_KEY_9) && input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->a = !(GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->a);
		}
	}

	// mouse
	if (input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		_isCameraControll = !_isCameraControll;
		glfwSetCursorPos(_window->getWindow(), _window->getWidth() / 2, _window->getHeight() / 2);

		_window->setCursorMode(_isCameraControll ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	if (input.isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT))
	{
		double xpos, ypos;
		glfwGetCursorPos(_window->getWindow(), &xpos, &ypos);
		ypos = _window->getHeight() - ypos;

		glm::vec3 rayStart;
		glm::vec3 rayDir;
		calculateRay(xpos, ypos, _activeCamera, rayStart, rayDir);

		// collision with model nodes
		std::list<RenderObject*>& renderObjects = GraphicsManager::getInstance().getRenderObjects();
		for (std::list<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i)
		{
			RenderObject* renderObject = *i;
			rayTestWithModelNode(renderObject, renderObject->getModelRootNode(), rayStart, rayDir, renderObject->getSceneObject()->getGlobalTransformMatrix());
		}
	}
}


void MainGameScene::terminate()
{

}


void MainGameScene::rayTestWithModelNode(RenderObject* renderObject, ModelNode* modelNode, glm::vec3 rayStart, glm::vec3 rayDir, glm::mat4 parentTransform)
{
	AABB* aabb = modelNode->getAABB();
	glm::mat4 modelMatrix = parentTransform * modelNode->getTransformMatrix();
	float distance;
	if (isRayIntersectOBB(rayStart, rayDir, *aabb, modelMatrix, distance))
	{
		glm::vec4 rayStartLocalspace = glm::inverse(modelMatrix) * glm::vec4(rayStart.x, rayStart.y, rayStart.z, 1.0f);
		glm::vec4 rayDirLocalspace = glm::inverse(modelMatrix) * glm::vec4(rayDir.x, rayDir.y, rayDir.z, 0.0f);

		if (distance > 0.0f)
		{
			ClickableObject* clickableObject = static_cast<ClickableObject*>(renderObject->getSceneObject()->getComponent(CT_CLICKABLE_OBJECT));
			if (clickableObject != NULL)
			{
				clickableObject->click(modelNode);
			}
		}
	}

	for (int i = 0; i < modelNode->getChildrenCount(); ++i)
	{
		rayTestWithModelNode(renderObject, modelNode->getChildren()[i], rayStart, rayDir, modelMatrix);
	}
}
