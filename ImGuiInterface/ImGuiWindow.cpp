#include "ImGuiWindow.h"


ImGuiWindow::ImGuiWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen)
    : _sceneManager(sceneManager), _buses(buses), _isOpen(isOpen)
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
