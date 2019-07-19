#ifndef EDITORGUI_H_INCLUDED
#define EDITORGUI_H_INCLUDED


#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

#include "EditorEvent.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Window/Window.h"


class EditorGUI
{
    private:
        std::shared_ptr<Window> _window;

        SceneManager* _sceneManager;

        SceneObject* _selectedSceneObject;

        std::list<EditorEvent> _events;

        bool _styleDark;

        bool _showObjectProperties;
        bool _showCameraFPSSettings;
        bool _showAddSceneObjectDialog;
        bool _showFileIODialog;
        bool _showDemo;
        bool _showTestWindow;
        bool _showOpenDialog;

        char _openDialogEditTextBuffer[1024];

        void initializeImGui();

        void drawMainMenu();
        void drawTestWindow();
        void drawSceneGraph();
        void drawObjectProperties();
        void drawOpenDialog();

        void inspectSceneObject(SceneObject* object = nullptr);

    public:
        EditorGUI(std::shared_ptr<Window> window, SceneManager* sceneManager);
        ~EditorGUI();

        void setSelectedSceneObject(SceneObject* sceneObject);
        SceneObject* getSelectedSceneObject();

        bool hasNextEvent();
        EditorEvent getNextEvent();

        bool GUIhasFocus();

        void draw();

};


#endif // EDITORGUI_H_INCLUDED
