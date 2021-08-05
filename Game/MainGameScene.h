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
		static constexpr char* PARAM_BUS_MODEL = "BUS_MODEL";
		static constexpr char* PARAM_BUS_CONFIGURATION = "BUS_CONFIGURATION";
		static constexpr char* PARAM_BUS_REPAINT = "BUS_REPAINT";

		GameState _state;

		std::vector<Bus*> _buses;
		Bus* _activeBus;
		std::vector<CameraFPS*> _cameras;
		CameraFPS* _activeCamera;

		Hud* _hud;
		ProgressBar* _progressBar;

		bool _isCameraControll;
		bool _isMirrorControll;
		int _mirrorControllIndex;

		void getBusConfigurationVariablesFromSceneParams(const std::unordered_map<std::string, std::string>& sceneParams,
														 std::unordered_map<std::string, std::string>& outBusConfigurationVariables);

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
		explicit MainGameScene(Window* window);
		virtual ~MainGameScene();

		static void createSceneParams(std::unordered_map<std::string, std::string>& outParams, const std::string& busModel, const std::string& busConfiguration, const std::string& busRepaint);
		static void createSceneParams(std::unordered_map<std::string, std::string>& outParams, const std::string& busModel, const std::string& busRepaint,
									  const std::unordered_map<std::string, std::string>& busConfigurationVariables);

		void initialize() override;

		void fixedStepUpdate(double deltaTime) override;
		void update(double deltaTime) override;


		void readInput(double deltaTime) override;
		void fixedStepReadInput(float deltaTime) override;

		void terminate() override;

};


#endif // MAINGAMESCENE_H_INCLUDED
