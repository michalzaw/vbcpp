set (IMGUI_INTERFACE_SOURCES
	ImguiInterface/ImguiInterface.cpp
	ImguiInterface/ImguiWindow.cpp
	ImguiInterface/BusLineAndDirectionWindow.cpp
	ImguiInterface/ColorsWindow.cpp
	ImguiInterface/MenuBar.cpp
	ImguiInterface/MenuSelectBusInterfaceWindow.cpp
	ImguiInterface/PhysicsDebuggerWindow.cpp
	ImguiInterface/VariablesWindow.cpp
)

set (IMGUI_INTERFACE_HEADERS
	ImguiInterface/ImguiInterface.h
	ImGuiInterface/ImGuiInterfaceContext.h
	ImguiInterface/ImguiWindow.h
	ImguiInterface/BusLineAndDirectionWindow.h
	ImguiInterface/ColorsWindow.h
	ImguiInterface/MenuBar.h
	ImguiInterface/MenuSelectBusInterfaceWindow.h
	ImguiInterface/PhysicsDebuggerWindow.h
	ImguiInterface/VariablesWindow.h
)

set (IMGUI_INTERFACE_SOURCES ${IMGUI_INTERFACE_SOURCES} PARENT_SCOPE)
set (IMGUI_INTERFACE_HEADERS ${IMGUI_INTERFACE_HEADERS} PARENT_SCOPE)