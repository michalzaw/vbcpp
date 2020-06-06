set (BUS_SOURCES
	Bus/BusConstraint.cpp
	Bus/BusLoader.cpp
	Bus/BusRaycast.cpp
	Bus/Desktop.cpp
	Bus/Door.cpp
	Bus/DoorClassic.cpp
	Bus/DoorSE.cpp
	Bus/DoorSimple.cpp
)

set (BUS_HEADERS
	Bus/Bus.h
	Bus/BusConstraint.h
	Bus/BusLoader.h
	Bus/BusRaycast.h
	Bus/Desktop.h
	Bus/Door.h
	Bus/DoorClassic.h
	Bus/DoorSE.h
	Bus/DoorSimple.h
)

set (BUS_SOURCES ${BUS_SOURCES} PARENT_SCOPE)
set (BUS_HEADERS ${BUS_HEADERS} PARENT_SCOPE)