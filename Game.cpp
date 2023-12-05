#include "Game.h"

#include <ctime>

#include "Bus/BusLoader.h"

#include "Game/CameraControlComponent.h"
#include "Game/GameLogicSystem.h"
#include "Game/GameConfig.h"

#include "Graphics/Renderer.h"

#include "ImGuiInterface/ImGuiInterfaceContext.h"

#include "Scene/SceneLoader.h"

#include "Utils/InputSystem.h"
#include "Utils/ResourceManager.h"
#include "Utils/RaycastingUtils.h"

#include "Window/Window.h"


Game* instance = nullptr;


Game::Game()
	: _window(nullptr), _backgroundWindow(nullptr),
	_fps(0),
	_loadingScene(""),
	_gameSceneName(""), _nextGameSceneName(""), _gameScene(nullptr), _nextGameScene(nullptr), _swapScenes(false),
	_useLoadingScreen(true)
{
	instance = this;
}


void Game::loadGameConfig()
{

}


void Game::createWindow()
{
	GameConfig& gameConfig = GameConfig::getInstance();

	_window = new Window;
	_window->createWindow(gameConfig.windowWidth, gameConfig.windowHeight, 10, 40, gameConfig.fullscreenMode, gameConfig.verticalSync, gameConfig.openGlDebugContext);
	_window->setWindowTitle(WINDOW_TITLE);
}


void Game::initializeEngineSystems()
{
	GameConfig& gameConfig = GameConfig::getInstance();

	InputSystem::getInstance().init(_window);

	OGLDriver::getInstance().initialize();

	Renderer& renderer = Renderer::getInstance();
	renderer.setFramebufferTextureFormat(gameConfig.hdrQuality == 32 ? TF_RGBA_32F : TF_RGBA_16F);
	renderer.setMsaaAntialiasing(gameConfig.msaaAntialiasing);
	renderer.setMsaaAntialiasingLevel(gameConfig.msaaAntialiasingLevel);
	renderer.setFxaa(gameConfig.isFxaaEnabled);
	renderer.setBloom(gameConfig.isBloomEnabled);
	renderer.setIsShadowMappingEnable(gameConfig.isShadowmappingEnable);
	renderer.init(_window->getWidth(), _window->getHeight());
	renderer.setDayNightRatio(1.0f);
	renderer.setAlphaToCoverage(true);
	renderer.setExposure(1.87022f);
	renderer.setToneMappingType(TMT_CLASSIC);
	renderer.t = 0;

	ImGuiInterfaceContext::initializeImGuiContext(_window, true);

#ifdef DRAW_IMGUI
	_physicsDebugRenderer = new PhysicsDebugRenderer;
#endif // DRAW_IMGUI
}


void Game::setFirstScene(const std::string& sceneName, bool useLoadingScreen, const std::unordered_map<std::string, std::string>& params)
{
	_gameSceneName = sceneName;
	_firstSceneParams = params;
	_useLoadingScreen = useLoadingScreen;
}


void Game::initialize()
{
	srand(static_cast<unsigned int>(time(NULL)));

	loadGameConfig();
	createWindow();
	initializeEngineSystems();

	if (_useLoadingScreen && !_loadingScene.empty())
	{
		_gameScene = getSceneByName(_loadingScene);
		_gameScene->setNextGameScene(_gameSceneName, false, _firstSceneParams);
	}
	else
	{
		_gameScene = getSceneByName(_gameSceneName);
		_gameScene->setParams(_firstSceneParams);
	}

	_gameScene->initialize();
	_initialized = true;
	Renderer::getInstance().setGraphicsManager(_gameScene->getGraphicsManager());
	Renderer::getInstance().rebuildStaticLighting();
#ifdef DRAW_IMGUI
	_gameScene->getPhysicsManager()->setDebugRenderer(_physicsDebugRenderer);
#endif // DRAW_IMGUI
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

	_gameScene->fixedStepUpdateScene(deltaTime);
}


GameScene* Game::getSceneByName(const std::string& name)
{
	auto createSceneFunction = _registeredScenes.find(name);
	if (createSceneFunction != _registeredScenes.end())
	{
		return createSceneFunction->second();
	}

	return nullptr;
}


void Game::asyncLoadScene(Window* window, GameScene* scene)
{
	glfwMakeContextCurrent(window->getWindow());

	scene->initialize();

	glFinish();

	glfwMakeContextCurrent(nullptr);
}


void Game::swapScenesImpl(float deltaTime)
{
	static float speed = -1;
	float sceneVisibility = Renderer::getInstance().getSceneVisibility();
	if (speed < 0 && sceneVisibility > 0.0f || speed > 0 && sceneVisibility < 1.0f)
	{
		sceneVisibility += speed * deltaTime;
		sceneVisibility = clamp(sceneVisibility, 0.0f, 1.0f);
		Renderer::getInstance().setSceneVisibility(sceneVisibility);
	}

	if (speed < 0 && sceneVisibility == 0.0f)
	{
		speed = -speed;
		performSwapScenes();
	}
	if (speed > 0 && sceneVisibility == 1)
	{
		speed = -speed;
		_swapScenes = false;
	}
}


void Game::performSwapScenes()
{
	_gameScene->terminate();
	delete _gameScene;

	_gameSceneName = _nextGameSceneName;
	_gameScene = _nextGameScene;
	_nextGameSceneName = "";
	_nextGameScene = nullptr;

	Renderer::getInstance().setGraphicsManager(_gameScene->getGraphicsManager());
	Renderer::getInstance().rebuildStaticLighting();
#ifdef DRAW_IMGUI
	_gameScene->getPhysicsManager()->setDebugRenderer(_physicsDebugRenderer);
#endif // DRAW_IMGUI
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

		//glfwPollEvents();

		//if (_state == GS_GAME)
		if (_initialized)
		{
			readInput(deltaTime);

			OGLDriver::getInstance().update();

			while (accumulator > TIME_STEP)
			{
				fixedStepUpdate(TIME_STEP);
				fixedStepReadInput(TIME_STEP);

				accumulator -= TIME_STEP;
			}

			_gameScene->updateScene(deltaTime);

			Renderer::getInstance().renderAll();


#ifdef DRAW_IMGUI
			if (_gameScene->getPhysicsManager()->getDebugRenderingState())
			{
				_physicsDebugRenderer->renderAll();
			}
			_gameScene->getImGuiInterface()->draw();
#endif // DRAW_IMGUI

			// Render GUI
			Renderer::getInstance().renderGUI(_gameScene->getGuiManager()->getGUIRenderList());
		}
		_window->swapBuffers();
		//_window->updateEvents();

		_nextGameSceneName = _gameScene->getNextScene();
		if (!_nextGameSceneName.empty() && _nextGameScene == nullptr)
		{
			if (_backgroundWindow == nullptr)
			{
				_backgroundWindow = new Window;
				_backgroundWindow->createInvisibleWindow(_window);
			}

			if (_gameScene->isUseLoadingScreenToLoadNextGameScene() && !_loadingScene.empty())
			{
				_nextGameScene = getSceneByName(_loadingScene);
				_nextGameScene->setNextGameScene(_nextGameSceneName, false, _gameScene->getNextSceneParams());
			}
			else
			{
				_nextGameScene = getSceneByName(_nextGameSceneName);
				_nextGameScene->setParams(_gameScene->getNextSceneParams());
			}

			_loadingSceneFuture = std::async(std::launch::async, &Game::asyncLoadScene, this, _backgroundWindow, _nextGameScene);
		}
		if (_nextGameScene != nullptr && _loadingSceneFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
		{
			_swapScenes = true;
		}
		if (_swapScenes)
		{
			swapScenesImpl(deltaTime);
		}
	}

}


void Game::terminate()
{
	_gameScene->terminateScene();
	delete _gameScene;

	_window->setCursorMode(GLFW_CURSOR_NORMAL);

	ImGuiInterfaceContext::destroyImGuiContext();

	delete _window;

	if (_backgroundWindow != nullptr)
	{
		delete _backgroundWindow;
	}
}


void Game::readInput(double deltaTime)
{
#ifdef DRAW_IMGUI
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
#endif // DRAW_IMGUI

	_gameScene->readInput(deltaTime);
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
		_gameScene->getImGuiInterface()->setIsOpen(!_gameScene->getImGuiInterface()->isOpen());
	}
#endif // DRAW_IMGUI

	if (input.isKeyPressed(GLFW_KEY_ESCAPE))
	{
		_window->setCloseFlag();
	}
	if (input.isKeyPressed(GLFW_KEY_ENTER))
	{
		/*_gameScene->terminateScene();
		delete _gameScene;

		//_sceneManager->clearScene();

		_gameScene = new MainGameScene(_window);
		_gameScene->initialize();*/
	}

	_gameScene->fixedStepReadInput(deltaTime);
}
