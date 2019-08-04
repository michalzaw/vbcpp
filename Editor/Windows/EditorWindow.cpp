#include "EditorWindow.h"


EditorWindow::EditorWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, bool isOpen, std::list<EditorEvent>* events)
    : _sceneManager(sceneManager), _selectedSceneObject(selectedSceneObject), _events(events),
    _isOpen(isOpen)
{

}


bool* EditorWindow::getOpenFlagPointer()
{
    return &_isOpen;
}


void EditorWindow::draw()
{
    if (_isOpen)
    {
        drawWindow();
    }
}
