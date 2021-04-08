#include "Game.h"

#include <ctime>
#include <thread>

#include "Bus/BusLoader.h"

#include "Game/CameraControlComponent.h"
#include "Game/GameLogicSystem.h"
#include "Game/GameConfig.h"
#include "Game/MainGameScene.h"

#include "Graphics/Renderer.h"

#include "Scene/SceneLoader.h"

#include "Utils/InputSystem.h"
#include "Utils/ResourceManager.h"
#include "Utils/Logger.h"
#include "Utils/RaycastingUtils.h"


Game* instance = nullptr;


Game::Game()
	: _window(nullptr),
	_physicsManager(nullptr), _soundManager(nullptr), _sceneManager(nullptr), _gui(nullptr), _imGuiInterface(nullptr),
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
	_window->createWindow(gameConfig.windowWidth, gameConfig.windowHeight, 10, 40, gameConfig.isFullscreen, gameConfig.verticalSync);
	_window->setWindowTitle(WINDOW_TITLE);
}


void Game::initializeEngineSystems()
{
	GameConfig& gameConfig = GameConfig::getInstance();

	InputSystem::getInstance().init(_window);

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
	renderer.setToneMappingType(TMT_ACES);
	renderer.t = 0;

	// unused
	GraphicsManager::getInstance().setWindDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	GraphicsManager::getInstance().setWindVelocity(0.6f);

	_gui = new GUIManager;

#ifdef DRAW_IMGUI
	_imGuiInterface = new ImGuiInterface(_window, _sceneManager);

	_physicsDebugRenderer = new PhysicsDebugRenderer;

	_physicsManager->setDebugRenderer(_physicsDebugRenderer);
#endif // DRAW_IMGUI
}


void Game::initialize()
{
	srand(static_cast<unsigned int>(time(NULL)));

	loadGameConfig();
	createWindow();
	initializeEngineSystems();

	_gameScene = new MainGameScene(_window, _physicsManager, _soundManager, _sceneManager, _gui);
	_gameScene->initialize();
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
	_gameScene->fixedStepUpdate(deltaTime);
	GraphicsManager::getInstance().update(deltaTime);

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

		//if (_state == GS_GAME)
		{
			readInput(deltaTime);

			while (accumulator > TIME_STEP)
			{
				fixedStepUpdate(TIME_STEP);

				accumulator -= TIME_STEP;
			}

			_soundManager->update();

			_gameScene->update(deltaTime);

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
	delete _gameScene;

	_physicsManager->stop();

	_window->setCursorMode(GLFW_CURSOR_NORMAL);

	delete _gui;

	GameLogicSystem::getInstance().destroy();

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
		_imGuiInterface->setIsOpen(!_imGuiInterface->isOpen());
	}
#endif // DRAW_IMGUI

	_gameScene->fixedStepReadInput(deltaTime);
}
