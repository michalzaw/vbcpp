#include "GameScene.h"


GameScene::GameScene(Window* window)
	: _window(window),
	_nextGameScene(this)
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


void GameScene::setNextGameScene(GameScene* gameScene)
{
	_nextGameScene = gameScene;
}


GameScene* GameScene::getNextScene()
{
	return _nextGameScene;
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
