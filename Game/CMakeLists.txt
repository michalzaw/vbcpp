set (GAME_SOURCES
	Game/AI/AIAgent.cpp
	Game/AI/AIAgentPhysicalVechicle.cpp
	Game/AI/AIAgentVehicle.cpp
	Game/AI/PathComponent.cpp
	Game/AI/StopComponent.cpp
	Game/AI/TrafficLightsComponent.cpp
	Game/MainGameScene/MapView.cpp
	Game/MainGameScene/MirrorImage.cpp
	Game/Transit/BusRoutesLoader.cpp
    Game/Transit/BusStopComponent.cpp
	Game/Transit/TransitSystem.cpp
	Game/Transit/ScheduleLoader.cpp
	Game/BusStartPoint.cpp
	Game/CameraControlComponent.cpp
	Game/GameClock.cpp
	Game/GameConfig.cpp
	Game/GameEnvironment.cpp
	Game/GameLogicSystem.cpp
	Game/GameScene.cpp
	Game/LuaGameContext.cpp
	Game/Hud.cpp
	Game/LoadingScreenScene.cpp
	Game/MainGameScene.cpp
	Game/MenuSelectBusScene.cpp
	Game/TestScene.cpp
)

set (GAME_HEADERS 
	Game/AI/AIAgent.h
	Game/AI/AIAgentPhysicalVechicle.h
	Game/AI/AIAgentVehicle.h
	Game/AI/PathComponent.h
	Game/AI/StopComponent.h
	Game/AI/TrafficLightsComponent.h
	Game/MainGameScene/MapView.h
	Game/MainGameScene/MirrorImage.h
	Game/Transit/BusRoutes.h
	Game/Transit/BusRoutesLoader.h
	Game/Transit/BusStopComponent.h
	Game/Transit/CurrentRouteData.h
	Game/Transit/TransitSystem.h
	Game/Transit/Schedule.h
	Game/Transit/ScheduleLoader.h
	Game/BusStartPoint.h
	Game/CameraControlComponent.h
	Game/Directories.h
	Game/GameClock.h
	Game/GameConfig.h
	Game/GameEnvironment.h
	Game/GameLogicSystem.h
	Game/GameScene.h
	Game/GameScenesNames.h
	Game/Hud.h
	Game/LuaGameContext.h
	Game/LoadingScreenScene.h
	Game/MainGameScene.h
	Game/MenuSelectBusScene.h
	Game/TestScene.h
    
)

set (GAME_SOURCES ${GAME_SOURCES} PARENT_SCOPE)
set (GAME_HEADERS ${GAME_HEADERS} PARENT_SCOPE)