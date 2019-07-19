#ifndef SCENEGRAPHWINDOW_H_INCLUDED
#define SCENEGRAPHWINDOW_H_INCLUDED


#include "EditorWindow.h"


class SceneGraphWindow : public EditorWindow
{
    private:
        virtual void drawWindow() override;

        void inspectSceneObject(SceneObject* object);

    public:
        SceneGraphWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject);
};


#endif // SCENEGRAPHWINDOW_H_INCLUDED
