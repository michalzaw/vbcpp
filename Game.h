#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


#include <string>
#include <thread>
#include <atomic>
#include <future>

#include "Game/Hud.h"
#include "Game/GameScene.h"

#include "Graphics/PhysicsDebugRenderer.h"

#include "GUI/GUIManager.h"

#include "ImGuiInterface/ImGuiInterface.h"

#include "Scene/SceneManager.h"

#include "Window/Window.h"


class Game
{
	private:
		static constexpr char* WINDOW_TITLE = "Virtual Bus Core++";

		Window* _window;

		PhysicsDebugRenderer* _physicsDebugRenderer;

		std::thread* _loadingThread;
		std::atomic<bool> _initialized;

		double _lastFPSupdate;
		int _numberOfFrames;
		int _fps;

		GameScene* _gameScene;
		GameScene* _nextGameScene;
		std::future<void> _loadingSceneFuture;
		std::atomic<bool> _loadingSceneDone;

		void loadGameConfig();
		void createWindow();
		void initializeEngineSystems();

		void updateFpsCounter(double timePhysicsCurr);
		void fixedStepUpdate(double deltaTime);

		void readInput(double deltaTime);
		void fixedStepReadInput(float deltaTime);

	public:
		Game();

		void initialize();

		void run();

		void terminate();

};


#endif // GAME_H_INCLUDED
