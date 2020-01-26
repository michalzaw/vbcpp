#include "Game.h"

#include <ctime>
#include <thread>

#include "Bus/BusLoader.h"

#include "Game/GameConfig.h"

#include "Graphics/Renderer.h"

#include "Scene/SceneLoader.h"

#include "Utils/ResourceManager.h"
#include "Utils/Logger.h"
#include "Utils/RaycastingUtils.h"


Game* instance = nullptr;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	instance->keyCallback(key, scancode, action, mods);
}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	instance->mouseButtonCallback(button, action, mods);
}


Game::Game()
	: _state(GS_LOADING),
	_window(nullptr),
	_physicsManager(nullptr), _soundManager(nullptr), _sceneManager(nullptr),
	_fps(0),
	_activeBus(nullptr),
	_activeCamera(nullptr),
	_isCameraControll(true), _isMirrorControll(false), _mirrorControllIndex(-1)
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
	_window->createWindow(gameConfig.windowWidth, gameConfig.windowHeight, 10, 40, gameConfig.isFullscreen);
	_window->setWindowTitle(WINDOW_TITLE);

	// Callbacki do obslugi zdarzen
	glfwSetKeyCallback(_window->getWindow(), ::keyCallback);
	glfwSetMouseButtonCallback(_window->getWindow(), ::mouseButtonCallback);
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
}


void Game::initScene()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("cam1");
	CameraFPS* cameraFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 10000);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(500.0f);
	cameraObject->setRotation(0, degToRad(-90), 0);
	cameraObject->setPosition(10, 7, -10);
	cameraObject->setPosition(0, 0, 0);

	setActiveCamera(cameraFPS);
}


void Game::setActiveCamera(CameraFPS* camera)
{
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

	_window->setCursorMode(GLFW_CURSOR_DISABLED);
}


void Game::initialize()
{
	srand(static_cast<unsigned int>(time(NULL)));

	loadGameConfig();
	createWindow();
	// inicjalizacja imgui
	initializeEngineSystems();
	initScene();

	loadScene();

	initGui();

	startGame();
}


void Game::fixedStepUpdate(double deltaTime)
{
	_physicsManager->simulate(deltaTime);
	_activeBus->update(deltaTime);
	GraphicsManager::getInstance().update(deltaTime);
	BusStopSystem::getInstance().update(deltaTime, _activeBus);
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
		//++numberOfFrames;

		timePhysicsCurr = glfwGetTime();
		double deltaTime = timePhysicsCurr - timePhysicsPrev;
		timePhysicsPrev = timePhysicsCurr;

		deltaTime = std::max(0.0, deltaTime);
		accumulator += deltaTime;
		accumulator = clamp(accumulator, 0.0, MAX_ACCUMULATED_TIME);

		if (_state == GS_GAME)
		{
			readInput(deltaTime);

			while (accumulator > TIME_STEP)
			{
				fixedStepUpdate(TIME_STEP);

				accumulator -= TIME_STEP;
			}

			_soundManager->update();

			_hud->update();

			Renderer::getInstance().renderAll();
		}

		// Render GUI
		Renderer::getInstance().renderGUI(_gui->getGUIRenderList());

		_window->swapBuffers();
		_window->updateEvents();
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
	destroyImGuiContext();
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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
	}

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

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		Renderer::getInstance().setExposure(Renderer::getInstance().getExposure() * 1.1f);
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		Renderer::getInstance().setExposure(Renderer::getInstance().getExposure() / 1.1f);
	}


	if (_isCameraControll)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, _window->getWidth() / 2, _window->getHeight() / 2);

		_activeCamera->setRotation(xpos, ypos);
	}
}


void Game::keyCallback(int key, int scancode, int action, int mods)
{
#ifdef DRAW_IMGUI
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
#endif // DRAW_IMGUI

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
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

	}

	else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		_activeBus->doorOpenClose(1);
	}

	else if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		_activeBus->doorOpenClose(2);
	}

	else if (key == GLFW_KEY_C && action == GLFW_PRESS)
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
	}

	else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
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

	// debug
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
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT)
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
	}
}
