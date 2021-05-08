set (BUS_SOURCES
	Bus/BusConfigurationsLoader.cpp
	Bus/BusConstraint.cpp
	Bus/BusLoader.cpp
	Bus/BusPreviewLoader.cpp
	Bus/BusRaycast.cpp
	Bus/Desktop.cpp
	Bus/Door.cpp
	Bus/DoorClassic.cpp
	Bus/DoorSE.cpp
	Bus/DoorSimple.cpp
)

set (BUS_HEADERS
	Bus/Bus.h
	Bus/BusConfigurationsLoader.h
	Bus/BusConstraint.h
	Bus/BusDescription.h
	Bus/BusLoader.h
	Bus/BusPreviewLoader.h
	Bus/BusRaycast.h
	Bus/Desktop.h
	Bus/Door.h
	Bus/DoorClassic.h
	Bus/DoorSE.h
	Bus/DoorSimple.h
)

set (BUS_SOURCES ${BUS_SOURCES} PARENT_SCOPE)
set (BUS_HEADERS ${BUS_HEADERS} PARENT_SCOPE)