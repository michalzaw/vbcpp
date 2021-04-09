set (GAME_SOURCES
    Game/BusStopComponent.cpp
	Game/BusStopSystem.cpp
	Game/CameraControlComponent.cpp
	Game/GameConfig.cpp
	Game/GameEnvironment.cpp
	Game/GameLogicSystem.cpp
	Game/Hud.cpp
	Game/MainGameScene.cpp
	Game/MenuSelectBusScene.cpp
)

set (GAME_HEADERS 
	Game/BusStopComponent.h
	Game/BusStopSystem.h
	Game/CameraControlComponent.h
	Game/Directories.h
	Game/GameConfig.h
	Game/GameEnvironment.h
	Game/GameLogicSystem.h
	Game/GameScene.h
	Game/Hud.h
	Game/MainGameScene.h
	Game/MenuSelectBusScene.h
    
)

set (GAME_SOURCES ${GAME_SOURCES} PARENT_SCOPE)
set (GAME_HEADERS ${GAME_HEADERS} PARENT_SCOPE)