#ifndef IMGUIINTERFACEI_H_INCLUDED
#define IMGUIINTERFACEI_H_INCLUDED

#include "BusLineAndDirectionWindow.h"
#include "ColorsWindow.h"
#include "PhysicsDebuggerWindow.h"
#include "VariablesWindow.h"
#include "MenuBar.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Window/Window.h"


class ImGuiInterface
{
    private:
        Window* _window;

        SceneManager* _sceneManager;

        bool _styleDark;
		bool _isOpen;

		BusLineAndDirectionWindow* _busLineAndDirectionWindow;
		ColorsWindow* _colorsWindow;
		PhysicsDebuggerWindow* _physicsDebuggerWindow;
        VariablesWindow* _variablesWindow;
        MenuBar* _menuBar;

        void initializeImGui();

    public:
		ImGuiInterface(Window* window, SceneManager* sceneManager);
        ~ImGuiInterface();
        
        bool GUIhasFocus();

		void setIsOpen(bool isOpen);
		bool isOpen();

        void draw();
};


#endif // IMGUIINTERFACEI_H_INCLUDED
