#include "ImGuiWindow.h"


ImGuiWindow::ImGuiWindow(SceneManager* sceneManager, bool isOpen)
    : _sceneManager(sceneManager), _isOpen(isOpen)
{

}


bool* ImGuiWindow::getOpenFlagPointer()
{
    return &_isOpen;
}


void ImGuiWindow::draw()
{
    if (_isOpen)
    {
        drawWindow();
    }
}
