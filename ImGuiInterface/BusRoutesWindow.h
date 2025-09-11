#ifndef BUSROUTESWINDOW_H_INCLUDED
#define BUSROUTESWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


struct BusRoutes;


class BusRoutesWindow : public ImGuiWindow
{
	private:
		std::vector<Bus*>* _buses;
		BusRoutes* _routes;

		virtual void drawWindow() override;

	public:
		BusRoutesWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, BusRoutes* routes, bool isOpen = true);
};


#endif // BUSROUTESWINDOW_H_INCLUDED
