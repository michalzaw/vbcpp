set (GAME_SOURCES
	Game/AI/AIAgent.cpp
	Game/AI/AIAgentPhysicalVechicle.cpp
	Game/AI/AIAgentVehicle.cpp
	Game/AI/PathComponent.cpp
	Game/AI/StopComponent.cpp
	Game/AI/TrafficLightsComponent.cpp
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
	Game/TestScene.cpp
	Game/MainGameScene/MirrorImage.cpp
)

set (GAME_HEADERS 
	Game/AI/AIAgent.h
	Game/AI/AIAgentPhysicalVechicle.h
	Game/AI/AIAgentVehicle.h
	Game/AI/PathComponent.h
	Game/AI/StopComponent.h
	Game/AI/TrafficLightsComponent.h
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
	Game/TestScene.h
	Game/MainGameScene/MirrorImage.h
    
)

set (GAME_SOURCES ${GAME_SOURCES} PARENT_SCOPE)
set (GAME_HEADERS ${GAME_HEADERS} PARENT_SCOPE)