#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


#include <string>
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
		Window* _backgroundWindow;

		PhysicsDebugRenderer* _physicsDebugRenderer;

		//std::thread* _loadingThread;
		std::atomic<bool> _initialized;

		double _lastFPSupdate;
		int _numberOfFrames;
		int _fps;

		std::unordered_map<std::string, std::function<GameScene*()>> _registeredScenes;

		std::string _gameSceneName;
		std::string _nextGameSceneName;
		GameScene* _gameScene;
		GameScene* _nextGameScene;
		std::future<void> _loadingSceneFuture;

		void loadGameConfig();
		void createWindow();
		void initializeEngineSystems();

		void updateFpsCounter(double timePhysicsCurr);
		void fixedStepUpdate(double deltaTime);

		void readInput(double deltaTime);
		void fixedStepReadInput(float deltaTime);

		template<typename T>
		GameScene* createScene();

		GameScene* getSceneByName(const std::string& name);

		void asyncLoadScene(Window* window, GameScene* scene);

	public:
		Game();

		template<typename T>
		void registerSceneType(const std::string& name);

		void setFirstScene(const std::string& sceneName);

		void initialize();

		void run();

		void terminate();

};


template<typename T>
GameScene* Game::createScene()
{
	return new T(_window);
}

template<typename T>
void Game::registerSceneType(const std::string& name)
{
	_registeredScenes.insert(std::make_pair(name, std::bind(&Game::createScene<T>, this)));
}


#endif // GAME_H_INCLUDED
