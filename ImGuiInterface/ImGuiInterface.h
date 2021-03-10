#ifndef IMGUIINTERFACEI_H_INCLUDED
#define IMGUIINTERFACEI_H_INCLUDED

#include "BusLineAndDirectionWindow.h"
#include "ColorsWindow.h"
#include "PhysicsDebuggerWindow.h"
#include "VariablesWindow.h"

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
		std::vector<Bus*>* _buses;

        bool _styleDark;
		bool _isOpen;

		BusLineAndDirectionWindow* _busLineAndDirectionWindow;
		ColorsWindow* _colorsWindow;
		PhysicsDebuggerWindow* _physicsDebuggerWindow;
        VariablesWindow* _variablesWindow;

        void initializeImGui();

        void drawMainMenu();

		void ShowTransformGizmo(CameraStatic* camera, SceneObject* obj);

    public:
		ImGuiInterface(Window* window, SceneManager* sceneManager, std::vector<Bus*>* buses);
        ~ImGuiInterface();
        
        bool GUIhasFocus();

		void setIsOpen(bool isOpen);
		bool isOpen();

        void draw();
};


#endif // IMGUIINTERFACEI_H_INCLUDED
