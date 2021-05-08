#ifndef IMGUIINTERFACEI_H_INCLUDED
#define IMGUIINTERFACEI_H_INCLUDED


#include <vector>

#include "ImGuiWindow.h"

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

        std::vector<ImGuiWindow*> _windows;

        void initializeImGui();

    public:
		ImGuiInterface(Window* window, SceneManager* sceneManager);
        ~ImGuiInterface();
        
        bool GUIhasFocus();

		void setIsOpen(bool isOpen);
		bool isOpen();

        void addWindow(ImGuiWindow* window);

        void draw();
};


#endif // IMGUIINTERFACEI_H_INCLUDED
