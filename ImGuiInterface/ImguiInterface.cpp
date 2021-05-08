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

	_busLineAndDirectionWindow = new BusLineAndDirectionWindow(_sceneManager);
	_colorsWindow = new ColorsWindow(_sceneManager);
	_physicsDebuggerWindow = new PhysicsDebuggerWindow(_sceneManager, false);
    _variablesWindow = new VariablesWindow(_sceneManager, false);

    std::vector<MenuItem> windowMenuItems;
    windowMenuItems.push_back(MenuItem("Bus line and direction", _busLineAndDirectionWindow->getOpenFlagPointer()));
    windowMenuItems.push_back(MenuItem("Colors", _colorsWindow->getOpenFlagPointer()));
    windowMenuItems.push_back(MenuItem("Physics debugger", _physicsDebuggerWindow->getOpenFlagPointer()));
    windowMenuItems.push_back(MenuItem("Game variables", _variablesWindow->getOpenFlagPointer()));

    std::vector<MenuItem> menuItems;
    menuItems.push_back(MenuItem("Window", windowMenuItems));

    _menuBar = new MenuBar(_sceneManager, menuItems);
}


ImGuiInterface::~ImGuiInterface()
{
	delete _busLineAndDirectionWindow;
	delete _colorsWindow;
	delete _physicsDebuggerWindow;
    delete _menuBar;

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

		_busLineAndDirectionWindow->draw();
		if (GameConfig::getInstance().developmentMode)
		{
			_colorsWindow->draw();
			_physicsDebuggerWindow->draw();
            _variablesWindow->draw();
            _menuBar->draw();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
