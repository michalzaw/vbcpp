set (IMGUI_INTERFACE_SOURCES
	ImguiInterface/ImguiInterface.cpp
	ImguiInterface/ImguiWindow.cpp
	ImguiInterface/BusLineAndDirectionWindow.cpp
	ImguiInterface/ColorsWindow.cpp
	ImguiInterface/PhysicsDebuggerWindow.cpp
)

set (IMGUI_INTERFACE_HEADERS
	ImguiInterface/ImguiInterface.h
	ImguiInterface/ImguiWindow.h
	ImguiInterface/BusLineAndDirectionWindow.h
	ImguiInterface/ColorsWindow.h
	ImguiInterface/PhysicsDebuggerWindow.h
)

set (IMGUI_INTERFACE_SOURCES ${IMGUI_INTERFACE_SOURCES} PARENT_SCOPE)
set (IMGUI_INTERFACE_HEADERS ${IMGUI_INTERFACE_HEADERS} PARENT_SCOPE)