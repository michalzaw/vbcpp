#ifndef EDITORWINDOW_H_INCLUDED
#define EDITORWINDOW_H_INCLUDED


#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_impl_glfw.h"
#include "../../ImGui/imgui_impl_opengl3.h"

#include "../EditorEvent.h"

#include "../../Graphics/Renderer.h"

#include "../../Scene/SceneManager.h"


class EditorWindow
{
    protected:
        SceneManager* _sceneManager;
        SceneObject*& _selectedSceneObject;

        std::list<EditorEvent>* _events;

        bool _isOpen;

        virtual void drawWindow() = 0;

    public:
        EditorWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, bool isOpen = true, std::list<EditorEvent>* events = nullptr);

        bool* getOpenFlagPointer();

        void draw();

};


#endif // EDITORWINDOW_H_INCLUDED
