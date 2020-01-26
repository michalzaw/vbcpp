#ifndef COLORSWINDOW_H_INCLUDED
#define COLORSWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


class ColorsWindow : public ImGuiWindow
{
	private:
		virtual void drawWindow() override;

	public:
		ColorsWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen = true);
};


#endif // COLORSWINDOW_H_INCLUDED
