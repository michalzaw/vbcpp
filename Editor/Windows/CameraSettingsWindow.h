#ifndef CAMERASETTINGSWINDOW_H_INCLUDED
#define CAMERASETTINGSWINDOW_H_INCLUDED


#include "EditorWindow.h"


class CameraSettingsWindow : public EditorWindow
{
    private:
        virtual void drawWindow() override;

    public:
        CameraSettingsWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, bool isOpen = false);

};


#endif // CAMERASETTINGSWINDOW_H_INCLUDED
