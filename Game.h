#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


#include <string>
#include <thread>
#include <atomic>

#include "Game/Hud.h"
#include "Game/GameScene.h"

#include "Graphics/PhysicsDebugRenderer.h"

#include "GUI/GUIManager.h"

#include "ImGuiInterface/ImGuiInterface.h"

#include "Scene/SceneManager.h"

#include "Window/Window.h"


#define DRAW_IMGUI


class Game
{
	private:
		static constexpr char* WINDOW_TITLE = "Virtual Bus Core++";

		Window* _window;

		GraphicsManager* _graphicsManager;
		PhysicsManager* _physicsManager;
		SoundManager* _soundManager;
		SceneManager* _sceneManager;
		GUIManager* _gui;
		ImGuiInterface* _imGuiInterface;
		PhysicsDebugRenderer* _physicsDebugRenderer;

		std::thread* _loadingThread;
		std::atomic<bool> _initialized;

		double _lastFPSupdate;
		int _numberOfFrames;
		int _fps;

		GameScene* _gameScene;

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
