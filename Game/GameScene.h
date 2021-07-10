#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED


#include "../GUI/GUIManager.h"

#include "../ImGuiInterface/ImGuiInterface.h"

#include "../Scene/SceneManager.h"

#include "../Window/Window.h"


#define DRAW_IMGUI


class GameScene
{
	private:
		std::string _nextGameScene;

	protected:
		Window* _window;

		GraphicsManager* _graphicsManager;
		PhysicsManager* _physicsManager;
		SoundManager* _soundManager;
		SceneManager* _sceneManager;
		GUIManager* _gui;
		ImGuiInterface* _imGuiInterface;

		void setNextGameScene(const std::string& gameSceneName);

	public:
		explicit GameScene(Window* window);
		virtual ~GameScene();

		void fixedStepUpdateScene(double deltaTime);
		void updateScene(double deltaTime);
		void terminateScene();

		GraphicsManager* getGraphicsManager() { return _graphicsManager; }
		PhysicsManager* getPhysicsManager() { return _physicsManager; }
		GUIManager* getGuiManager() { return _gui; }
		ImGuiInterface* getImGuiInterface() { return _imGuiInterface; }

		const std::string& getNextScene();

		virtual void initialize() = 0;

		virtual void fixedStepUpdate(double deltaTime) = 0;
		virtual void update(double deltaTime) = 0;

		virtual void readInput(double deltaTime) = 0;
		virtual void fixedStepReadInput(float deltaTime) = 0;

		virtual void terminate() = 0;

};


#endif // GAMESCENE_H_INCLUDED
