#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED


#include "GameScene.h"


class Hud;


enum GameState
{
	GS_LOADING,
	GS_GAME
};


class MainGameScene final : public GameScene
{
	private:
		GameState _state;

		Bus* _activeBus;
		std::vector<CameraFPS*> _cameras;
		CameraFPS* _activeCamera;

		Hud* _hud;

		bool _isCameraControll;
		bool _isMirrorControll;
		int _mirrorControllIndex;

		CameraFPS* createCameraBusDriver();
		CameraFPS* createCameraBus();
		CameraFPS* createCameraFPSGlobal();

		void initScene();
		void setActiveCamera(CameraFPS* camera);
		void loadScene();
		void initGui();
		void initImGuiInterface();
		void startGame();

		void rayTestWithModelNode(RenderObject* renderObject, ModelNode* modelNode, glm::vec3 rayStart, glm::vec3 rayDir, glm::mat4 parentTransform = glm::mat4(1.0f));

	public:
		MainGameScene(Window* window, GraphicsManager* graphicsManager, PhysicsManager* physicsManager, SoundManager* soundManager, SceneManager* sceneManager, GUIManager* gui, ImGuiInterface* imGuiInterface);
		virtual ~MainGameScene();

		void initialize() override;

		void fixedStepUpdate(double deltaTime) override;
		void update(double deltaTime) override;


		void readInput(double deltaTime) override;
		void fixedStepReadInput(float deltaTime) override;

		void terminate() override;

};


#endif // MAINGAMESCENE_H_INCLUDED
