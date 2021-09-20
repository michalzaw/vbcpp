#include "Game.h"

#include "Game/GameScenesNames.h"
#include "Game/MainGameScene.h"
#include "Game/MenuSelectBusScene.h"
#include "Game/LoadingScreenScene.h"
#include "Game/TestScene.h"

#include "Utils/Logger2.h"


void testLogger()
{
	GameConfig& gameConfig = GameConfig::getInstance();

	LOG_DEBUG("Start application");
	LOG_INFO("Game initialization");
	LOG_WARNING("Cannot load texture");
	LOG_ERROR("Cannot initialize OpenGL context");

	float zmienna12_1 = 2.43f;
	glm::vec4 test(1.0f, 2.0f, 3.4f, 1.2345f);
	glm::mat4 mat(1.0f);
	std::string str2 = "Hello world";

	LOG_INFO(LOG_VARIABLE(zmienna12_1));
	LOG_INFO(LOG_VARIABLE(test));
	LOG_INFO(LOG_VARIABLE(mat));
	LOG_INFO(LOG_VARIABLE(str2));

	LOG_INFO(LOG_VARIABLE(gameConfig));
	system("pause");
}


int main()
{
	GameConfig& gameConfig = GameConfig::getInstance();
	gameConfig.loadGameConfig("game.xml");


#ifdef DEVELOPMENT_RESOURCES
	gameConfig.loadDevelopmentConfig("devSettings.xml");
	ResourceManager::getInstance().setAlternativeResourcePath(gameConfig.alternativeResourcesPath);
#endif // DEVELOPMENT_RESOURCES


	Logger2::init(getLogLevelFromString(gameConfig.loggerLevel), gameConfig.loggerConsoleOutput, !gameConfig.loggerFileOutput.empty(), gameConfig.loggerFileOutput);

	testLogger();

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

	return 0;
}
