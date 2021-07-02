#include "ImGuiInterface.h"

#include "../Graphics/Renderer.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imGizmo.h"

#include "glm/gtc/type_ptr.hpp"

ImGuiInterface::ImGuiInterface(SceneManager* sceneManager)
	: _sceneManager(sceneManager),
    _isOpen(false)
{
    
}


ImGuiInterface::~ImGuiInterface()
{
    for (int i = 0; i < _windows.size(); ++i)
    {
        delete _windows[i];
    }
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
