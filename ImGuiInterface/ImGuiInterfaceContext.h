#ifndef IMGUIINTERFACECONTEXT_H_INCLUDED
#define IMGUIINTERFACECONTEXT_H_INCLUDED


#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../Window/Window.h"


namespace ImGuiInterfaceContext
{
	void initializeImGuiContext(Window* window, bool styleDark = true)
	{
        ImGui::CreateContext();

        if (styleDark)
            ImGui::StyleColorsDark();
        else
            ImGui::StyleColorsLight();

        ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 130");

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("fonts/arial.ttf", 20.0f);
        io.Fonts->AddFontDefault();
	}

	void destroyImGuiContext()
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}
}


#endif // IMGUIINTERFACECONTEXT_H_INCLUDED
