#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED


#include <memory>

#include "EditorGUI.h"

#include "../Graphics/OGLDriver.h"
#include "../Graphics/Renderer.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Window/Window.h"


class Editor
{
    private:
        std::shared_ptr<Window> _window;

        PhysicsManager* _physicsManager;
        SoundManager* _soundManager;
        SceneManager* _sceneManager;

        std::unique_ptr<EditorGUI> _editorGUI;

        bool _cameraActive;
        CameraFPS* _camera;
        SceneObject* _cameraObject;

        SceneObject* _axisObject;

        bool createWindow();
        void initializeEngineSubsystems();
        void clearScene();

    public:
        Editor();
        ~Editor();

        std::shared_ptr<Window> getWindow();

        void run();

        void processInput(double deltaTime);

        void mouseButtonCallback(int button, int action, int mods);
        void keyCallback(int key, int scancode, int action, int mods);
        void charCallback(unsigned int c);
        void changeWindowSizeCallback(int width, int height);
        void changeFramebufferSizeCallback(int width, int height);

};


#endif // EDITOR_H_INCLUDED
