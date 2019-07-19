#ifndef NEWDIALOGWINDOW_H_INCLUDED
#define NEWDIALOGWINDOW_H_INCLUDED


#include "EditorWindow.h"


class NewDialogWindow : public EditorWindow
{
    private:
        char _inputTextNameBuffer[1024];
        char _inputTextAuthorBuffer[1024];

        virtual void drawWindow() override;

    public:
        NewDialogWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::list<EditorEvent>* events, bool isOpen = false);

        void open();

};


#endif // NEWDIALOGWINDOW_H_INCLUDED
