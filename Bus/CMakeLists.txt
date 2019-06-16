set (BUS_SOURCES
	Bus/BusConstraint.cpp
	Bus/BusRaycast.cpp
	Bus/Desktop.cpp
)

set (BUS_HEADERS
	Bus/Bus.h
	Bus/BusConstraint.h
	Bus/BusRaycast.h
	Bus/Desktop.h
	Bus/Door.h
	Bus/DoorSE.h
	Bus/DoorSimple.h
)

set (BUS_SOURCES ${BUS_SOURCES} PARENT_SCOPE)
set (BUS_HEADERS ${BUS_HEADERS} PARENT_SCOPE)