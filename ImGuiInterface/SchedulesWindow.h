#ifndef SCHEDULESWINDOW_H_INCLUDED
#define SCHEDULESWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


struct Schedule;
class BusStopComponent;


class SchedulesWindow : public ImGuiWindow
{
	private:
		std::vector<Bus*>* _buses;
		Schedule* _schedules;

		int _selectedLineIndex;
		int _selectedBrigadeIndex;
		int _selectedRouteIndex;

		BusStopComponent* findBusStopById(int id);

		virtual void drawWindow() override;

	public:
		SchedulesWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, Schedule* schedules, bool isOpen = true);

};


#endif // SCHEDULESWINDOW_H_INCLUDED
