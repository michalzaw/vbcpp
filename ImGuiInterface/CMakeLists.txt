set (IMGUI_INTERFACE_SOURCES
	ImGuiInterface/ImGuiInterface.cpp
	ImGuiInterface/ImGuiWindow.cpp
	ImGuiInterface/BusLineAndDirectionWindow.cpp
	ImGuiInterface/ColorsWindow.cpp
	ImGuiInterface/MenuBar.cpp
	ImGuiInterface/MenuSelectBusInterfaceWindow.cpp
	ImGuiInterface/PhysicsDebuggerWindow.cpp
	ImGuiInterface/VariablesWindow.cpp
)

set (IMGUI_INTERFACE_HEADERS
	ImGuiInterface/ImGuiInterface.h
	ImGuiInterface/ImGuiInterfaceContext.h
	ImGuiInterface/ImGuiWindow.h
	ImGuiInterface/BusLineAndDirectionWindow.h
	ImGuiInterface/ColorsWindow.h
	ImGuiInterface/MenuBar.h
	ImGuiInterface/MenuSelectBusInterfaceWindow.h
	ImGuiInterface/PhysicsDebuggerWindow.h
	ImGuiInterface/VariablesWindow.h
)

set (IMGUI_INTERFACE_SOURCES ${IMGUI_INTERFACE_SOURCES} PARENT_SCOPE)
set (IMGUI_INTERFACE_HEADERS ${IMGUI_INTERFACE_HEADERS} PARENT_SCOPE)