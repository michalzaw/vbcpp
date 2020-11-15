#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


#include <string>

#include "Game/Hud.h"

#include "Graphics/PhysicsDebugRenderer.h"

#include "GUI/GUIManager.h"

#include "ImGuiInterface/ImGuiInterface.h"

#include "Scene/SceneManager.h"

#include "Window/Window.h"


#define DRAW_IMGUI


enum GameState
{
	GS_LOADING,
	GS_GAME
};


class Game
{
	private:
		static constexpr char* WINDOW_TITLE = "Virtual Bus Core++";

		GameState _state;

		Window* _window;

		PhysicsManager* _physicsManager;
		SoundManager* _soundManager;
		SceneManager* _sceneManager;
		GUIManager* _gui;
		ImGuiInterface* _imGuiInterface;
		PhysicsDebugRenderer* _physicsDebugRenderer;

		double _lastFPSupdate;
		int _numberOfFrames;
		int _fps;

		std::vector<Bus*> _buses;
		Bus* _activeBus;
		CameraFPS* _activeCamera;

		Hud* _hud;

		bool _isCameraControll;
		bool _isMirrorControll;
		int _mirrorControllIndex;

		void loadGameConfig();
		void createWindow();
		void initializeEngineSystems();
		void initScene();
		void setActiveCamera(CameraFPS* camera);
		void loadScene();
		void initGui();
		void startGame();

		void updateFpsCounter(double timePhysicsCurr);
		void fixedStepUpdate(double deltaTime);

		void readInput(double deltaTime);

		void rayTestWithModelNode(RenderObject* renderObject, ModelNode* modelNode, glm::vec3 rayStart, glm::vec3 rayDir, glm::mat4 parentTransform = glm::mat4(1.0f));

	public:
		Game();

		void initialize();

		void run();

		void terminate();

		void keyCallback(int key, int scancode, int action, int mods);
		void mouseButtonCallback(int button, int action, int mods);
};


#endif // GAME_H_INCLUDED
