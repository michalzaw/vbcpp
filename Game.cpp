#include "Game.h"

#include <ctime>
#include <thread>

#include "Bus/BusLoader.h"

#include "Game/CameraControlComponent.h"
#include "Game/GameLogicSystem.h"
#include "Game/GameConfig.h"
#include "Game/MainGameScene.h"
#include "Game/MenuSelectBusScene.h"
#include "Game/TestScene.h"

#include "Graphics/Renderer.h"

#include "ImGuiInterface/ImGuiInterfaceContext.h"

#include "Scene/SceneLoader.h"

#include "Utils/InputSystem.h"
#include "Utils/ResourceManager.h"
#include "Utils/Logger.h"
#include "Utils/RaycastingUtils.h"

#include "Window/Window.h"


Game* instance = nullptr;


Game::Game()
	: _window(nullptr),
	_initialized(false),
	_fps(0)
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
	_window->createWindow(gameConfig.windowWidth, gameConfig.windowHeight, 10, 40, gameConfig.fullscreenMode, gameConfig.verticalSync);
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
	renderer.setBloom(gameConfig.isBloomEnabled);
	renderer.setIsShadowMappingEnable(gameConfig.isShadowmappingEnable);
	renderer.init(_window->getWidth(), _window->getHeight());
	renderer.setDayNightRatio(1.0f);
	renderer.setAlphaToCoverage(true);
	renderer.setExposure(1.87022f);
	renderer.setToneMappingType(TMT_CLASSIC);
	renderer.t = 0;

	ImGuiInterfaceContext::initializeImGuiContext(_window, true);
}


void loadingThread(Window* window, GameScene* scene, std::atomic<bool>& initialized)
{
	glfwMakeContextCurrent(window->getWindow());

	scene->initialize();

	initialized = true;
}


void Game::initialize()
{
	srand(static_cast<unsigned int>(time(NULL)));

	loadGameConfig();
	createWindow();
	initializeEngineSystems();

	_gameScene = new MainGameScene(_window);
	//_gameScene = new MenuSelectBusScene(_window);
	//_gameScene = new TestScene(_window);
	
	Window* backgroundWindow = new Window;
	backgroundWindow->createInvisibleWindow(_window);

	//_loadingThread = new std::thread(loadingThread, backgroundWindow, _gameScene, std::ref(_initialized));
	//_loadingThread->detach();

	_gameScene->initialize();
	_initialized = true;
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
		}

#ifdef DRAW_IMGUI
		/*if (_physicsManager->getDebugRenderingState())
		{
			_physicsDebugRenderer->renderAll();
		}*/
		_gameScene->getImGuiInterface()->draw();
#endif // DRAW_IMGUI

		// Render GUI
		Renderer::getInstance().renderGUI(_gameScene->getGuiManager()->getGUIRenderList());

		_window->swapBuffers();
		//_window->updateEvents();
	}

}


void Game::terminate()
{
	_gameScene->terminateScene();
	delete _gameScene;

	_window->setCursorMode(GLFW_CURSOR_NORMAL);

	ImGuiInterfaceContext::destroyImGuiContext();

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
		_gameScene->terminateScene();
		delete _gameScene;

		//_sceneManager->clearScene();

		_gameScene = new MainGameScene(_window);
		_gameScene->initialize();
	}

	_gameScene->fixedStepReadInput(deltaTime);
}
