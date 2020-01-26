#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED


#include "../GUI/GUIManager.h"


class Bus;


class Hud
{
	private:
		GUIManager* _gui;

		Image* _hudImage;

		Label* _labelSpeed;
		Label* _labelrpm;
		Label* _labelFuel;

		Label* _labelGear;

		Label* _labelPassengers;
		Label* _labelPassengersGettingOff;

		Image* _imageEngineControl;
		Image* _imageHeadlightsControl;
		Image* _imageStopControl;

		std::vector<Image*> _imagesDoorClosed;
		std::vector<Image*> _imagesDoorOpened;


		Label* _labelBusStop;
		Label* _labelBusStop2;
		Label* _labelStats;


		Bus* _bus;

		std::string getGearAsString(int gear);
		std::string createStatsString();

	public:
		Hud(GUIManager* gui, Bus* bus);
		~Hud();

		void update();
		
};


#endif // HUD_H_INCLUDED