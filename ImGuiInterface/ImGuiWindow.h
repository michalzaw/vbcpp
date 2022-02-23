#ifndef EDITORWINDOW_H_INCLUDED
#define EDITORWINDOW_H_INCLUDED


#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../../Graphics/Renderer.h"

#include "../../Scene/SceneManager.h"


class ImGuiWindow
{
    protected:
        SceneManager* _sceneManager;

        bool _isOpen;

        virtual void drawWindow() = 0;

    public:
		ImGuiWindow(SceneManager* sceneManager, bool isOpen = true);

        bool* getOpenFlagPointer();

        void draw();

};


#endif // EDITORWINDOW_H_INCLUDED
