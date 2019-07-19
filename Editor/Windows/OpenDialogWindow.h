#ifndef OPENDIALOGWINDOW_H_INCLUDED
#define OPENDIALOGWINDOW_H_INCLUDED


#include "EditorWindow.h"


class OpenDialogWindow : public EditorWindow
{
    private:
        char _inputTextNameBuffer[1024];

        virtual void drawWindow() override;

    public:
        OpenDialogWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::list<EditorEvent>* events, bool isOpen = false);

        void open();

};


#endif // OPENDIALOGWINDOW_H_INCLUDED
