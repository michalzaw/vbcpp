#ifndef MENUSELECTBUSINTERFACEWINDOW_H_INCLUDED
#define MENUSELECTBUSINTERFACEWINDOW_H_INCLUDED


//#include <functional>

#include "ImGuiWindow.h"


class BusPreview;


class MenuSelectBusInterfaceWindow : public ImGuiWindow
{
	private:
		BusPreview* _busPreview;
		std::unordered_map<std::string, std::string>* _selectedBusConfigurationVariables;
		std::string& _selectedBusRepaintName;

		void drawWindow() override;

		void drawBusConfigurations();
		void drawBusRepaints();
		void drawStartButton();

		void changeBusConfiguration(int index);

	public:
		MenuSelectBusInterfaceWindow(std::unordered_map<std::string, std::string>* selectedBusConfigurationVariables, std::string& selectedBusRepaintName, SceneManager* sceneManager, bool isOpen = true);

		void setCurrentBusPreview(BusPreview* busPreview);

};


#endif // MENUSELECTBUSINTERFACEWINDOW_H_INCLUDED
