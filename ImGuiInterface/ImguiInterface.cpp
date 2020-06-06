#include "ImGuiInterface.h"

#include "../Graphics/Renderer.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imGizmo.h"

#include "glm/gtc/type_ptr.hpp"

ImGuiInterface::ImGuiInterface(Window* window, SceneManager* sceneManager, std::vector<Bus*>* buses)
	: _window(window), _sceneManager(sceneManager), _buses(buses),
    _styleDark(true), _isOpen(false)
{
    initializeImGui();

	_busLineAndDirectionWindow = new BusLineAndDirectionWindow(_sceneManager, _buses);
	_colorsWindow = new ColorsWindow(_sceneManager, _buses);
}


ImGuiInterface::~ImGuiInterface()
{
	delete _busLineAndDirectionWindow;
	delete _colorsWindow;

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
    io.Fonts->AddFontFromFileTTF("fonts/arial.ttf", 13.0f);
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


void ImGuiInterface::draw()
{
	if (_isOpen)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		drawMainMenu();

		_busLineAndDirectionWindow->draw();
		if (GameConfig::getInstance().developmentMode)
		{
			_colorsWindow->draw();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}


void ImGuiInterface::drawMainMenu()
{
    if( ImGui::BeginMainMenuBar() )
    {
        if (ImGui::BeginMenu("Windows"))
        {
            ImGui::MenuItem("Bus line and direction", NULL, _busLineAndDirectionWindow->getOpenFlagPointer());
            ImGui::MenuItem("Colors", NULL, _colorsWindow->getOpenFlagPointer());
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
