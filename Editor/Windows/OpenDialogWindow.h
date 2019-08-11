#ifndef OPENDIALOGWINDOW_H_INCLUDED
#define OPENDIALOGWINDOW_H_INCLUDED


#include "EditorWindow.h"


class OpenDialogWindow : public EditorWindow
{
    private:
        std::vector<std::string> _availableMapsNames;
        int _listBoxMapSelectedItem;

		bool _openModalDialog;

        virtual void drawWindow() override;

    public:
        OpenDialogWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::list<EditorEvent>* events, bool isOpen = false);

        void open();

};


#endif // OPENDIALOGWINDOW_H_INCLUDED
