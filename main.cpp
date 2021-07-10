#include "Game.h"

#include "Game/GameScenesNames.h"
#include "Game/MainGameScene.h"
#include "Game/MenuSelectBusScene.h"
#include "Game/TestScene.h"


int main()
{
	Game game;

	game.registerSceneType<MainGameScene>(GameScenesNames::MAIN_SCENE);
	game.registerSceneType<MenuSelectBusScene>(GameScenesNames::MENU_SELECT_BUS);
	game.registerSceneType<TestScene>(GameScenesNames::TEST_SCENE);

	game.setFirstScene(GameScenesNames::MENU_SELECT_BUS);

	game.initialize();
	game.run();
	game.terminate();

	return 0;
}
