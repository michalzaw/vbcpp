#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED


#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

#include "../Graphics/OGLDriver.h"
#include "../Graphics/Renderer.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Window/Window.h"


class Editor
{
    private:
        Window* _window;

        PhysicsManager* _physicsManager;
        SoundManager* _soundManager;
        SceneManager* _sceneManager;

        SceneObject* _selectedSceneObject;

        bool _styleDark;

        bool _showObjectProperties;
        bool _showCameraFPSSettings;
        bool _showAddSceneObjectDialog;
        bool _showFileIODialog;
        bool _showDemo;
        bool _showTestWindow;

        bool createWindow();
        void initializeEngineSubsystems();
        void initializeImGui();

        void renderImGuiFrame();
        void drawMainMenu();
        void drawTestWindow();
        void drawSceneGraph();
        void drawObjectProperties();

        void inspectSceneObject(SceneObject* object = nullptr);

    public:
        Editor();
        ~Editor();

        void run();

};


#endif // EDITOR_H_INCLUDED
