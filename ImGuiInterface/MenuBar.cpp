#include "MenuBar.h"


MenuBar::MenuBar(SceneManager* sceneManager, std::vector<MenuItem>& menuItems)
	: ImGuiWindow(sceneManager, true),
	_menuItems(menuItems)
{

}


void MenuBar::drawWindow()
{
    if (ImGui::BeginMainMenuBar())
    {
        drawMenu(_menuItems);

        ImGui::EndMainMenuBar();
    }
}


void MenuBar::drawMenu(const std::vector<MenuItem>& menuItems)
{
    for (const MenuItem& menuItem : menuItems)
    {
        if (menuItem.subMenuItems.size() > 0)
        {
            if (ImGui::BeginMenu(menuItem.title.c_str()))
            {
                drawMenu(menuItem.subMenuItems);

                ImGui::EndMenu();
            }
        }
        else
        {
            ImGui::MenuItem(menuItem.title.c_str(), NULL, menuItem.selectedFlag);
        }
    }
}
