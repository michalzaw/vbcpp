set (GAME_SOURCES
    Game/BusStopComponent.cpp
	Game/BusStopSystem.cpp
	Game/GameConfig.cpp
)

set (GAME_HEADERS 
	Game/BusStopComponent.h
	Game/BusStopSystem.h
	Game/Directories.h
	Game/GameConfig.h
    
)

set (GAME_SOURCES ${GAME_SOURCES} PARENT_SCOPE)
set (GAME_HEADERS ${GAME_HEADERS} PARENT_SCOPE)