#include "NewDialogWindow.h"


NewDialogWindow::NewDialogWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::list<EditorEvent>* events, bool isOpen)
    : EditorWindow(sceneManager, selectedSceneObject, isOpen, events),
    _inputTextNameBuffer(""), _inputTextAuthorBuffer("")
{

}


void NewDialogWindow::drawWindow()
{
    if (ImGui::Begin("New map", &_isOpen))
    {
        ImGui::InputText("Map name", _inputTextNameBuffer, IM_ARRAYSIZE(_inputTextNameBuffer));
        ImGui::InputText("Author", _inputTextAuthorBuffer, IM_ARRAYSIZE(_inputTextAuthorBuffer));

        if (ImGui::Button("Create"))
        {
            _events->push_back(EditorEvent(EET_NEW_CLICK));

            _isOpen = false;
        }
    }
    ImGui::End();
}


void NewDialogWindow::open()
{
    memset(_inputTextNameBuffer, 0, strlen(_inputTextNameBuffer));
    memset(_inputTextAuthorBuffer, 0, strlen(_inputTextAuthorBuffer));

    _isOpen = true;
}
