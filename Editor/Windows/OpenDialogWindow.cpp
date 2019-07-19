#include "OpenDialogWindow.h"


OpenDialogWindow::OpenDialogWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::list<EditorEvent>* events, bool isOpen)
    : EditorWindow(sceneManager, selectedSceneObject, isOpen, events),
    _inputTextNameBuffer("")
{

}


void OpenDialogWindow::drawWindow()
{
    if (ImGui::Begin("Open map", &_isOpen))
    {
        ImGui::InputText("Map name", _inputTextNameBuffer, IM_ARRAYSIZE(_inputTextNameBuffer));

        if (ImGui::Button("Open"))
        {
            std::string mapName = (_inputTextNameBuffer);
            mapName = trim(mapName);

            _events->push_back(EditorEvent(EET_OPEN_CLICK, mapName));

            _isOpen = false;
        }
    }
    ImGui::End();
}


void OpenDialogWindow::open()
{
    memset(_inputTextNameBuffer, 0, strlen(_inputTextNameBuffer));

    _isOpen = true;
}
