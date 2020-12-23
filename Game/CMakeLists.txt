set (GAME_SOURCES
    Game/BusStopComponent.cpp
	Game/BusStopSystem.cpp
	Game/CameraControlComponent.cpp
	Game/CameraControlSystem.cpp
	Game/GameConfig.cpp
	Game/Hud.cpp
)

set (GAME_HEADERS 
	Game/BusStopComponent.h
	Game/BusStopSystem.h
	Game/CameraControlComponent.h
	Game/CameraControlSystem.h
	Game/Directories.h
	Game/GameConfig.h
	Game/Hud.h
    
)

set (GAME_SOURCES ${GAME_SOURCES} PARENT_SCOPE)
set (GAME_HEADERS ${GAME_HEADERS} PARENT_SCOPE)