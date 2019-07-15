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

        bool createWindow();
        void initializeEngineSubsystems();

    public:
        Editor();
        ~Editor();

        void run();

};


#endif // EDITOR_H_INCLUDED
