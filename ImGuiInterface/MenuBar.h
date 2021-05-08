#ifndef MENUBAR_H_INCLUDED
#define MENUBAR_H_INCLUDED


#include "ImGuiWindow.h"


struct MenuItem final
{
	std::string title;
	bool* selectedFlag;
	std::vector<MenuItem> subMenuItems;

	MenuItem(const std::string& title, bool* selectedFlag)
		: title(title), selectedFlag(selectedFlag)
	{}

	MenuItem(const std::string& title, const std::vector<MenuItem>& subMenuItems)
		: title(title), selectedFlag(nullptr), subMenuItems(subMenuItems)
	{}
};


class MenuBar final : public ImGuiWindow
{
	private:
		std::vector<MenuItem> _menuItems;

		void drawWindow() override;
		void drawMenu(const std::vector<MenuItem>& menuItems);

	public:
		MenuBar(SceneManager* sceneManager, std::vector<MenuItem>& menuItems);

};


#endif // MENUBAR_H_INCLUDED
