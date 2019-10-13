set (IMGUI_SOURCES
	ImGui/imgui.cpp
	ImGui/imgui_demo.cpp
	ImGui/imgui_draw.cpp
	ImGui/imgui_impl_glfw.cpp
	ImGui/imgui_impl_opengl3.cpp
	ImGui/imgui_widgets.cpp
	ImGui/imGizmo.cpp
)

set (IMGUI_HEADERS 
	ImGui/imconfig.h
	ImGui/imgui.h
	ImGui/imgui_impl_glfw.h
	ImGui/imgui_impl_opengl3.h
	ImGui/imgui_internal.h
	ImGui/imstb_rectpack.h
	ImGui/imstb_textedit.h
	ImGui/imstb_truetype.h
	ImGui/imGizmo.h
)

set (IMGUI_SOURCES ${IMGUI_SOURCES} PARENT_SCOPE)
set (IMGUI_HEADERS ${IMGUI_HEADERS} PARENT_SCOPE)