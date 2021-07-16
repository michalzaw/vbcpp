#include "Game.h"

#include "Game/GameScenesNames.h"
#include "Game/MainGameScene.h"
#include "Game/MenuSelectBusScene.h"
#include "Game/TestScene.h"


int main()
{
	GameConfig& gameConfig = GameConfig::getInstance();
	gameConfig.loadGameConfig("game.xml");

#ifdef DEVELOPMENT_RESOURCES
	gameConfig.loadDevelopmentConfig("devSettings.xml");
	ResourceManager::getInstance().setAlternativeResourcePath(gameConfig.alternativeResourcesPath);
#endif // DEVELOPMENT_RESOURCES

	Game game;

	game.registerSceneType<MainGameScene>(GameScenesNames::MAIN_SCENE);
	game.registerSceneType<MenuSelectBusScene>(GameScenesNames::MENU_SELECT_BUS);
	game.registerSceneType<TestScene>(GameScenesNames::TEST_SCENE);

	std::unordered_map<std::string, std::string> params;
	MainGameScene::createSceneParams(params, GameConfig::getInstance().busModel, GameConfig::getInstance().busConfiguration, GameConfig::getInstance().busRepaint);
	game.setFirstScene(GameScenesNames::MENU_SELECT_BUS, params);

	game.initialize();
	game.run();
	game.terminate();

	return 0;
}
