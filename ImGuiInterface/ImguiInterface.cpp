#include "ImGuiInterface.h"

#include "../Graphics/Renderer.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imGizmo.h"

#include "glm/gtc/type_ptr.hpp"

ImGuiInterface::ImGuiInterface(Window* window, SceneManager* sceneManager)
	: _window(window), _sceneManager(sceneManager),
    _styleDark(true), _isOpen(false)
{
    initializeImGui();
}


ImGuiInterface::~ImGuiInterface()
{
    for (int i = 0; i < _windows.size(); ++i)
    {
        delete _windows[i];
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void ImGuiInterface::initializeImGui()
{
    ImGui::CreateContext();

    if (_styleDark)
        ImGui::StyleColorsDark();
    else
        ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(_window->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("fonts/arial.ttf", 20.0f);
    io.Fonts->AddFontDefault();
}


bool ImGuiInterface::GUIhasFocus()
{
    return (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard);
}


void ImGuiInterface::setIsOpen(bool isOpen)
{
	_isOpen = isOpen;
}


bool ImGuiInterface::isOpen()
{
	return _isOpen;
}


void ImGuiInterface::addWindow(ImGuiWindow* window)
{
    _windows.push_back(window);
}


void ImGuiInterface::draw()
{
	if (_isOpen)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        for (ImGuiWindow* window : _windows)
        {
            window->draw();
        }

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
