#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED


#include "../GUI/GUIManager.h"

#include "../Scene/SceneManager.h"

#include "../Window/Window.h"


class GameScene
{
	protected:
		Window* _window;

		PhysicsManager* _physicsManager;
		SoundManager* _soundManager;
		SceneManager* _sceneManager;
		GUIManager* _gui;
		//Window* _window;

	public:
		GameScene(Window* window, PhysicsManager* physicsManager, SoundManager* soundManager, SceneManager* sceneManager, GUIManager* gui)
			: _window(window),
			_physicsManager(physicsManager), _soundManager(soundManager), _sceneManager(sceneManager), _gui(gui)
		{

		}
		virtual ~GameScene() {}

		virtual void initialize() = 0;

		virtual void fixedStepUpdate(double deltaTime) = 0;
		virtual void update(double deltaTime) = 0;

		virtual void readInput(double deltaTime) = 0;
		virtual void fixedStepReadInput(float deltaTime) = 0;

		virtual void terminate() = 0;

};


#endif // GAMESCENE_H_INCLUDED
