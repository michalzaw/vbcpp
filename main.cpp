#include "Game.h"

#include "Game/GameScenesNames.h"
#include "Game/MainGameScene.h"
#include "Game/MenuSelectBusScene.h"
#include "Game/LoadingScreenScene.h"
#include "Game/TestScene.h"

#include "Utils/Logger.h"


int main()
{
	GameConfig& gameConfig = GameConfig::getInstance();
	gameConfig.loadGameConfig("game.xml");


#ifdef DEVELOPMENT_RESOURCES
	gameConfig.loadDevelopmentConfig("devSettings.xml");
	ResourceManager::getInstance().setAlternativeResourcePath(gameConfig.alternativeResourcesPath);
#endif // DEVELOPMENT_RESOURCES


	Logger::init(getLogLevelFromString(gameConfig.loggerLevel), gameConfig.loggerConsoleOutput, !gameConfig.loggerFileOutput.empty(), gameConfig.loggerFileOutput);
	LOG_INFO("Start game: VirtualBus Core++");


	Game game;

	game.registerSceneType<MainGameScene>(GameScenesNames::MAIN_SCENE);
	game.registerSceneType<MenuSelectBusScene>(GameScenesNames::MENU_SELECT_BUS);
	game.registerSceneType<TestScene>(GameScenesNames::TEST_SCENE);
	game.registerLoadingSceneType<LoadingScreenScene>(GameScenesNames::LOADING_SCREEN);

	std::string firstSceneName = GameScenesNames::MENU_SELECT_BUS;
	bool useLoadingScreen = true;
#ifdef DEVELOPMENT_RESOURCES
	firstSceneName = !gameConfig.firstScene.empty() ? gameConfig.firstScene : firstSceneName;
	useLoadingScreen = gameConfig.useLoadingScreen;
#endif // DEVELOPMENT_RESOURCES
	std::unordered_map<std::string, std::string> params;
	MainGameScene::createSceneParams(params, GameConfig::getInstance().busModel, GameConfig::getInstance().busConfiguration, GameConfig::getInstance().busRepaint);
	game.setFirstScene(firstSceneName, useLoadingScreen, params);

	game.initialize();
	game.run();
	game.terminate();

	Logger::destroy();

	return 0;
}
