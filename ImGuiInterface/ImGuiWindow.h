#ifndef EDITORWINDOW_H_INCLUDED
#define EDITORWINDOW_H_INCLUDED


#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_impl_glfw.h"
#include "../../ImGui/imgui_impl_opengl3.h"

#include "../../Graphics/Renderer.h"

#include "../../Scene/SceneManager.h"


class ImGuiWindow
{
    protected:
        SceneManager* _sceneManager;
		std::vector<Bus*>* _buses;

        bool _isOpen;

        virtual void drawWindow() = 0;

    public:
		ImGuiWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen = true);

        bool* getOpenFlagPointer();

        void draw();

};


#endif // EDITORWINDOW_H_INCLUDED
