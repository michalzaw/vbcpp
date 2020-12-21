#include "Game.h"

#include <ctime>
#include <thread>

#include "Bus/BusLoader.h"

#include "Game/CameraControlSystem.h"
#include "Game/GameConfig.h"

#include "Graphics/Renderer.h"

#include "Scene/SceneLoader.h"

#include "Utils/InputSystem.h"
#include "Utils/ResourceManager.h"
#include "Utils/Logger.h"
#include "Utils/RaycastingUtils.h"


enum GameCamera
{
	GC_DRIVER,
	GC_BUS,
	GC_GLOBAL
};


Game* instance = nullptr;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	instance->keyCallback(key, scancode, action, mods);
}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	instance->mouseButtonCallback(button, action, mods);
}


void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	instance->scrollCallback(xOffset, yOffset);
}


Game::Game()
	: _state(GS_LOADING),
	_window(nullptr),
	_physicsManager(nullptr), _soundManager(nullptr), _sceneManager(nullptr), _gui(nullptr), _imGuiInterface(nullptr),
	_fps(0),
	_activeBus(nullptr),
	_activeCamera(nullptr),
	_hud(nullptr),
	_isCameraControll(false), _isMirrorControll(false), _mirrorControllIndex(-1)
{
	instance = this;
}


void Game::loadGameConfig()
{
	GameConfig& gameConfig = GameConfig::getInstance();
	gameConfig.loadGameConfig("game.xml");

#ifdef DEVELOPMENT_RESOURCES
	gameConfig.loadDevelopmentConfig("devSettings.xml");
	ResourceManager::getInstance().setAlternativeResourcePath(gameConfig.alternativeResourcesPath);
#endif // DEVELOPMENT_RESOURCES
}


void Game::createWindow()
{
	GameConfig& gameConfig = GameConfig::getInstance();

	_window = new Window;
	_window->createWindow(gameConfig.windowWidth, gameConfig.windowHeight, 10, 40, gameConfig.isFullscreen, gameConfig.verticalSync);
	_window->setWindowTitle(WINDOW_TITLE);

	InputSystem::getInstance().init(_window);
	// Callbacki do obslugi zdarzen
	//glfwSetKeyCallback(_window->getWindow(), ::keyCallback);
	//glfwSetMouseButtonCallback(_window->getWindow(), ::mouseButtonCallback);
	//glfwSetScrollCallback(_window->getWindow(), ::scrollCallback);
}


void Game::initializeEngineSystems()
{
	GameConfig& gameConfig = GameConfig::getInstance();

	OGLDriver::getInstance().initialize();

	_physicsManager = new PhysicsManager;
	_soundManager = new SoundManager();
	_soundManager->setMute(true);
	_sceneManager = new SceneManager(_physicsManager, _soundManager);

	Renderer& renderer = Renderer::getInstance();
	renderer.setFramebufferTextureFormat(gameConfig.hdrQuality == 32 ? TF_RGBA_32F : TF_RGBA_16F);
	renderer.setMsaaAntialiasing(gameConfig.msaaAntialiasing);
	renderer.setMsaaAntialiasingLevel(gameConfig.msaaAntialiasingLevel);
	renderer.setBloom(gameConfig.isBloomEnabled);
	renderer.setIsShadowMappingEnable(gameConfig.isShadowmappingEnable);
	renderer.init(_window->getWidth(), _window->getHeight());
	renderer.setDayNightRatio(1.0f);
	renderer.setAlphaToCoverage(true);
	renderer.setExposure(1.87022f);
	renderer.setToneMappingType(TMT_CLASSIC);
	renderer.t = 0;

	// unused
	GraphicsManager::getInstance().setWindDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	GraphicsManager::getInstance().setWindVelocity(0.6f);

	_gui = new GUIManager;

#ifdef DRAW_IMGUI
	_imGuiInterface = new ImGuiInterface(_window, _sceneManager, &_buses);

	_physicsDebugRenderer = new PhysicsDebugRenderer;

	_physicsManager->setDebugRenderer(_physicsDebugRenderer);
#endif // DRAW_IMGUI
}


CameraFPS* createCameraBusDriver(SceneManager* sceneManager)
{
	SceneObject* cameraObject = sceneManager->addSceneObject("cameraBusDriver");

	CameraFPS* cameraFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(0, 0, 0);
	cameraObject->setPosition(0, 0, 0);

	CameraControlComponent* cameraControlComponent = CameraControlSystem::getInstance().addCameraControlComponent(cameraFPS);
	cameraObject->addComponent(cameraControlComponent);
	cameraControlComponent->setMovmentControl(false);

	return cameraFPS;
}


CameraFPS* createCameraBus(SceneManager* sceneManager)
{
	SceneObject* cameraObject = sceneManager->addSceneObject("cameraBus");

	CameraFPS* cameraFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(degToRad(0.0f), degToRad(165.0f), degToRad(0.0f));
	cameraObject->setPosition(0, 0, 0);

	cameraFPS->setMinPositionOffset(1.0f);
	cameraFPS->setMaxPositionOffset(30.0f);
	cameraFPS->setPositionOffset(10.0f);

	CameraControlComponent* cameraControlComponent = CameraControlSystem::getInstance().addCameraControlComponent(cameraFPS);
	cameraObject->addComponent(cameraControlComponent);
	cameraControlComponent->setMovmentControl(false);

	return cameraFPS;
}


CameraFPS* createCameraFPSGlobal(SceneManager* sceneManager)
{
	SceneObject* cameraObject = sceneManager->addSceneObject("cameraFPSGlobal");

	CameraFPS* cameraFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(0, 0, 0);
	cameraObject->setPosition(0, 0, 0);

	CameraControlComponent* cameraControlComponent = CameraControlSystem::getInstance().addCameraControlComponent(cameraFPS);
	cameraObject->addComponent(cameraControlComponent);

	return cameraFPS;
}


void Game::initScene()
{
	_cameras.resize(3);

	_cameras[GC_DRIVER] = createCameraBusDriver(_sceneManager);
	_cameras[GC_BUS] = createCameraBus(_sceneManager);
	_cameras[GC_GLOBAL] = createCameraFPSGlobal(_sceneManager);

	setActiveCamera(_cameras[GC_BUS]);
}


void Game::setActiveCamera(CameraFPS* camera)
{
	if (_activeCamera != nullptr)
	{
		CameraControlSystem::getInstance().setCameraActivity(static_cast<CameraControlComponent*>(_activeCamera->getSceneObject()->getComponent(CT_CAMERA_CONTROL)), false);
	}
	CameraControlSystem::getInstance().setCameraActivity(static_cast<CameraControlComponent*>(camera->getSceneObject()->getComponent(CT_CAMERA_CONTROL)), true);

	_activeCamera = camera;

	GraphicsManager::getInstance().setCurrentCamera(camera);
	_soundManager->setActiveCamera(camera);
}


void Game::loadScene()
{
	BusLoader busLoader(_sceneManager, _physicsManager, _soundManager);
	Bus* bus = busLoader.loadBus(GameConfig::getInstance().busModel);
	_buses.push_back(bus);
	_activeBus = bus;

	SceneLoader sceneLoader(_sceneManager);
	sceneLoader.loadMap(GameConfig::getInstance().mapFile);

	bus->getSceneObject()->setPosition(_sceneManager->getBusStart().position);
	bus->getSceneObject()->setRotation(degToRad(_sceneManager->getBusStart().rotation.x),
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


	Renderer::getInstance().bakeStaticShadows();
}


void Game::initGui()
{
	_hud = new Hud(_gui, _activeBus);
}


void Game::startGame()
{
	_state = GS_GAME;

	_physicsManager->play();
	_soundManager->setMute(false);

	glfwSetCursorPos(_window->getWindow(), _window->getWidth() / 2, _window->getHeight() / 2);
}


void Game::initialize()
{
	srand(static_cast<unsigned int>(time(NULL)));

	loadGameConfig();
	createWindow();
	initializeEngineSystems();
	initScene();

	loadScene();

	initGui();

	startGame();
}


void Game::updateFpsCounter(double timePhysicsCurr)
{
	++_numberOfFrames;

	if (timePhysicsCurr - _lastFPSupdate >= 1.0)
	{
		_fps = _numberOfFrames;
		_numberOfFrames = 0;
		_lastFPSupdate += 1.0f;

		std::string newWindowTitle = std::string(WINDOW_TITLE) + " | FPS: " + toString(_fps);
		_window->setWindowTitle(newWindowTitle);
	}
}


void Game::fixedStepUpdate(double deltaTime)
{
	InputSystem::getInstance().update();

	_physicsManager->simulate(deltaTime);
	_activeBus->update(deltaTime);
	GraphicsManager::getInstance().update(deltaTime);
	BusStopSystem::getInstance().update(deltaTime, _activeBus);

	if (_isCameraControll)
	{
		CameraControlSystem::getInstance().update(deltaTime);
	}

	fixedStepReadInput(deltaTime);
}


void Game::run()
{
	const double TIME_STEP = 1 / 60.0;
	const double MAX_ACCUMULATED_TIME = 1.0;

	double accumulator = 0.0;

	double timePhysicsCurr;
	double timePhysicsPrev;
	timePhysicsPrev = timePhysicsCurr = glfwGetTime();

	while (_window->isOpened())
	{
		timePhysicsCurr = glfwGetTime();
		double deltaTime = timePhysicsCurr - timePhysicsPrev;
		timePhysicsPrev = timePhysicsCurr;

		deltaTime = std::max(0.0, deltaTime);
		accumulator += deltaTime;
		accumulator = clamp(accumulator, 0.0, MAX_ACCUMULATED_TIME);

		updateFpsCounter(timePhysicsCurr);

		if (_state == GS_GAME)
		{
			readInput(deltaTime);

			while (accumulator > TIME_STEP)
			{
				fixedStepUpdate(TIME_STEP);

				accumulator -= TIME_STEP;
			}

			_soundManager->update();

			_hud->update(_fps);

			Renderer::getInstance().renderAll();
		}

#ifdef DRAW_IMGUI
		if (_physicsManager->getDebugRenderingState())
		{
			_physicsDebugRenderer->renderAll();
		}
		_imGuiInterface->draw();
#endif // DRAW_IMGUI

		// Render GUI
		Renderer::getInstance().renderGUI(_gui->getGUIRenderList());

		_window->swapBuffers();
		//_window->updateEvents();
	}

}


void Game::terminate()
{
	delete _hud;

	_physicsManager->stop();

	_window->setCursorMode(GLFW_CURSOR_NORMAL);

	delete _gui;

	for (int i = 0; i < _buses.size(); ++i)
	{
		_buses[i]->drop();
	}

	_soundManager->drop();
	_physicsManager->drop();
	delete _sceneManager;

#ifdef DRAW_IMGUI
	delete _physicsDebugRenderer;
	delete _imGuiInterface;
#endif // DRAW_IMGUI

	delete _window;
}


void Game::readInput(double deltaTime)
{
#ifdef DRAW_IMGUI
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
#endif // DRAW_IMGUI

	GLFWwindow* window = _window->getWindow();

	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		_activeCamera->moveForward(deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		_activeCamera->moveBackward(deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		_activeCamera->strafeRight(deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		_activeCamera->strafeLeft(deltaTime);
	}*/

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


	/*if (_isCameraControll)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, _window->getWidth() / 2, _window->getHeight() / 2);

		_activeCamera->setRotation(xpos, ypos);
	}*/
}


void Game::fixedStepReadInput(float deltaTime)
{
	InputSystem& input = InputSystem::getInstance();

#ifdef DRAW_IMGUI
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
	if (input.isKeyPressed(GLFW_KEY_GRAVE_ACCENT))
	{
		_imGuiInterface->setIsOpen(!_imGuiInterface->isOpen());
	}
#endif // DRAW_IMGUI

	if (input.isKeyPressed(GLFW_KEY_ESCAPE))
	{
		_window->setCloseFlag();
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
		_activeBus->doorOpenClose(1);
	}

	if (input.isKeyPressed(GLFW_KEY_X))
	{
		_activeBus->doorOpenClose(2);
	}

	if (input.isKeyPressed(GLFW_KEY_C))
	{
		_activeBus->doorOpenClose(3);
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
		instance->_activeBus->toggleHandbrake();
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


void Game::keyCallback(int key, int scancode, int action, int mods)
{/*
#ifdef DRAW_IMGUI
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
	if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
	{
		_imGuiInterface->setIsOpen(!_imGuiInterface->isOpen());
	}
#endif // DRAW_IMGUI
	*/
	/*if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		_window->setCloseFlag();
	}

	else if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		_activeBus->setIsEnableHeadlights(!_activeBus->isEnableHeadlights());
	}
	else if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		_activeBus->setIsEnableLights(!_activeBus->isEnableLights());
	}
	else if (key == GLFW_KEY_H && action == GLFW_PRESS)
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

	}*/

	/*else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		_activeBus->doorOpenClose(1);
	}

	else if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		_activeBus->doorOpenClose(2);
	}

	else if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		_activeBus->doorOpenClose(3);
	}*/

	/*else if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (_activeCamera->getSceneObject()->hasParent())
		{
			_activeCamera->getSceneObject()->setPosition(_activeCamera->getPosition());
			_activeCamera->getSceneObject()->removeParent();
		}
		else
		{
			_activeBus->getSceneObject()->addChild(_activeCamera->getSceneObject());
			_activeCamera->getSceneObject()->setPosition(_activeBus->getDriverPosition());
			_activeCamera->getSceneObject()->setRotation(0, 0, 0);
		}
	}*/

	/*else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
	{
		_activeBus->getGearbox()->shiftUp();
	}
	else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
	{
		_activeBus->getGearbox()->shiftDown();
	}
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
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

	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (!_physicsManager->isRunning())
			_physicsManager->play();
		else
			_physicsManager->stop();
	}

	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		instance->_activeBus->toggleHandbrake();
	}

	else if (key == GLFW_KEY_M && action == GLFW_PRESS)
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
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		_activeBus->getSceneObject()->setPosition(_sceneManager->getBusStart().position);
		_activeBus->getSceneObject()->setRotation(degToRad(_sceneManager->getBusStart().rotation.x),
												  degToRad(_sceneManager->getBusStart().rotation.y),
												  degToRad(_sceneManager->getBusStart().rotation.z));
	}
	else if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
	{
		_cameras[GC_DRIVER]->getSceneObject()->setRotation(0.0f, 0.0f, 0.0f);

		setActiveCamera(_cameras[GC_DRIVER]);
	}
	else if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
	{
		_cameras[GC_BUS]->getSceneObject()->setRotation(0.0f, 0.0f, 0.0f);

		setActiveCamera(_cameras[GC_BUS]);
	}
	else if (key == GLFW_KEY_F7 && action == GLFW_PRESS)
	{
		_cameras[GC_GLOBAL]->getSceneObject()->setPosition(_activeCamera->getPosition());

		setActiveCamera(_cameras[GC_GLOBAL]);
	}

	// debug
	if (GameConfig::getInstance().developmentMode)
	{
		if (key == GLFW_KEY_1 && action == GLFW_PRESS && glfwGetKey(_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			ResourceManager::getInstance().reloadAllShaders();
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS && glfwGetKey(_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			ResourceManager::getInstance().reloadAllTextures();
		}
		if (key == GLFW_KEY_3 && action == GLFW_PRESS && glfwGetKey(_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			Renderer::getInstance().toogleRenderAABBFlag();
		}
		if (key == GLFW_KEY_4 && action == GLFW_PRESS && glfwGetKey(_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			Renderer::getInstance().toogleRenderOBBFlag();
		}
		if (key == GLFW_KEY_5 && action == GLFW_PRESS && glfwGetKey(_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			Renderer::getInstance().setAlphaToCoverage(!(Renderer::getInstance().isAlphaToCoverageEnable()));
		}
		if (key == GLFW_KEY_8 && action == GLFW_PRESS && glfwGetKey(_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			Renderer::getInstance().setBloom(!(Renderer::getInstance().isBloomEnable()));
		}
		if (key == GLFW_KEY_9 && action == GLFW_PRESS && glfwGetKey(_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->a = !(GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->a);
		}
	}*/
}


void Game::rayTestWithModelNode(RenderObject* renderObject, ModelNode* modelNode, glm::vec3 rayStart, glm::vec3 rayDir, glm::mat4 parentTransform)
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


void Game::mouseButtonCallback(int button, int action, int mods)
{
	/*if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		_isCameraControll = !_isCameraControll;
		glfwSetCursorPos(_window->getWindow(), _window->getWidth() / 2, _window->getHeight() / 2);

		_window->setCursorMode(_isCameraControll ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
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
	}*/
}


void Game::scrollCallback(double xOffset, double yOffset)
{
	/*if (_isCameraControll)
	{
		float newOffset = _activeCamera->getPositionOffset() + yOffset;
		newOffset = clamp(newOffset, 1.0f, 30.0f);
		_activeCamera->setPositionOffset(newOffset);
	}*/
}
