#ifndef MENUSELECTBUSINTERFACEWINDOW_H_INCLUDED
#define MENUSELECTBUSINTERFACEWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


class BusPreview;


class MenuSelectBusInterfaceWindow : public ImGuiWindow
{
	private:
		BusPreview* _busPreview;

		void drawWindow() override;

		void drawBusConfigurations();

	public:
		MenuSelectBusInterfaceWindow(SceneManager* sceneManager, bool isOpen = true);

		void setCurrentBusPreview(BusPreview* busPreview);

};


#endif // MENUSELECTBUSINTERFACEWINDOW_H_INCLUDED
