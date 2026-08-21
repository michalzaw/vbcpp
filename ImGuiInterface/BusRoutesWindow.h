#ifndef BUSROUTESWINDOW_H_INCLUDED
#define BUSROUTESWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


class BusRoutesWindow : public ImGuiWindow
{
	private:
		std::vector<Bus*>* _buses;

		virtual void drawWindow() override;

	public:
		BusRoutesWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen = true);

};


#endif // BUSROUTESWINDOW_H_INCLUDED
