set (GAME_SOURCES
	Game/AIAgent.cpp
	Game/AIAgentPhysicalVechicle.cpp
	Game/BusStartPoint.cpp
    Game/BusStopComponent.cpp
	Game/BusStopSystem.cpp
	Game/CameraControlComponent.cpp
	Game/GameConfig.cpp
	Game/GameEnvironment.cpp
	Game/GameLogicSystem.cpp
	Game/GameScene.cpp
	Game/Hud.cpp
	Game/LoadingScreenScene.cpp
	Game/MainGameScene.cpp
	Game/MenuSelectBusScene.cpp
	Game/PathComponent.cpp
	Game/TestScene.cpp
)

set (GAME_HEADERS 
	Game/AIAgent.h
	Game/AIAgentPhysicalVechicle.h
	Game/BusStartPoint.h
	Game/BusStopComponent.h
	Game/BusStopSystem.h
	Game/CameraControlComponent.h
	Game/Directories.h
	Game/GameConfig.h
	Game/GameEnvironment.h
	Game/GameLogicSystem.h
	Game/GameScene.h
	Game/GameScenesNames.h
	Game/Hud.h
	Game/LoadingScreenScene.h
	Game/MainGameScene.h
	Game/MenuSelectBusScene.h
	Game/PathComponent.h
	Game/TestScene.h
    
)

set (GAME_SOURCES ${GAME_SOURCES} PARENT_SCOPE)
set (GAME_HEADERS ${GAME_HEADERS} PARENT_SCOPE)