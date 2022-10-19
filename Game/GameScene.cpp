#include "GameScene.h"


GameScene::GameScene(Window* window)
	: _window(window),
	_nextGameScene("")
{
	_graphicsManager = new GraphicsManager;
	_physicsManager = new PhysicsManager;
	_soundManager = new SoundManager();
	_soundManager->setMute(true);
	_sceneManager = new SceneManager(_graphicsManager, _physicsManager, _soundManager);

	// unused
	_graphicsManager->setWindDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	_graphicsManager->setWindVelocity(0.6f);

	_gui = new GUIManager;

#ifdef DRAW_IMGUI
	_imGuiInterface = new ImGuiInterface(_sceneManager);
#endif // DRAW_IMGUI
}


GameScene::~GameScene()
{

}


void GameScene::setNextGameScene(const std::string& gameSceneName, bool useLoadingScreenToLoadNextGameScene, const std::unordered_map<std::string, string>& params)
{
	_nextGameScene = gameSceneName;
	_nextGameSceneParams = params;
	_useLoadingScreenToLoadNextGameScene = useLoadingScreenToLoadNextGameScene;
}


void GameScene::setParams(const std::unordered_map<std::string, string>& params)
{
	_params = params;
}


const std::string& GameScene::getNextScene()
{
	return _nextGameScene;
}


bool GameScene::isUseLoadingScreenToLoadNextGameScene()
{
	return _useLoadingScreenToLoadNextGameScene;
}


const std::unordered_map<std::string, std::string>& GameScene::getNextSceneParams()
{
	return _nextGameSceneParams;
}


void GameScene::fixedStepUpdateScene(double deltaTime)
{
	_physicsManager->simulate(deltaTime);
	fixedStepUpdate(deltaTime);
	_graphicsManager->update(deltaTime);

	_gui->update(deltaTime);
}


void GameScene::updateScene(double deltaTime)
{
	_soundManager->update();

	update(deltaTime);
}


void GameScene::terminateScene()
{
	terminate();

	_physicsManager->stop();

	delete _gui;

	_soundManager->drop();
	_physicsManager->drop();
	delete _sceneManager;

#ifdef DRAW_IMGUI
	//delete _physicsDebugRenderer;
	delete _imGuiInterface;
#endif // DRAW_IMGUI*/
}
