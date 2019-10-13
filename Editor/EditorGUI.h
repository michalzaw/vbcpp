#ifndef EDITORGUI_H_INCLUDED
#define EDITORGUI_H_INCLUDED

#include <queue>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

#include "EditorEvent.h"
#include "EditorContext.h"
#include "Windows/SceneGraphWindow.h"
#include "Windows/ObjectPropertiesWindow.h"
#include "Windows/NewDialogWindow.h"
#include "Windows/OpenDialogWindow.h"
#include "Windows/CameraSettingsWindow.h"
#include "Windows/ObjectDefinitionsWindow.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Window/Window.h"


class EditorGUI
{
    private:
        std::shared_ptr<Window> _window;

        SceneManager* _sceneManager;

        SceneObject* _selectedSceneObject;

		//std::shared_ptr<EditorContext> _editorContext;
        std::list<EditorEvent> _events;

        bool _styleDark;

        std::unique_ptr<SceneGraphWindow> _sceneGraphWindow;
        std::unique_ptr<ObjectPropertiesWindow> _objectPropertiesWindow;
        std::unique_ptr<NewDialogWindow> _newDialogWindow;
        std::unique_ptr<OpenDialogWindow> _openDialogWindow;
        std::unique_ptr<CameraSettingsWindow> _cameraSettingsWindow;
		std::unique_ptr<ObjectDefinitionsWindow> _objectDefinitionsWindow;

        bool _showDemo;

        void initializeImGui();

        void drawMainMenu();

		void showObjectProperties();
		void showObjectNameEdit();
		void showObjectTransformEdit();

		void ShowTransformGizmo(CameraStatic* camera, SceneObject* obj);

    public:
        EditorGUI(Window* window, SceneManager* sceneManager/*, std::shared_ptr<EditorContext> editorContext*/);
        ~EditorGUI();

        void setSelectedSceneObject(SceneObject* sceneObject);
        SceneObject* getSelectedSceneObject();

        bool hasNextEvent();
        EditorEvent getNextEvent();

        bool GUIhasFocus();

        void draw();
};


#endif // EDITORGUI_H_INCLUDED
