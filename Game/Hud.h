#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED


#include "../GUI/GUIManager.h"


class Bus;
class GameClock;


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

		Label* _labelTime;


		Label* _labelBusStop;
		Label* _labelBusStop2;
		Label* _labelStats;


		Bus* _bus;
		GameClock* _gameClock;

		std::string getGearAsString(int gear);
		std::string createStatsString(int fps);

	public:
		Hud(GUIManager* gui, Bus* bus, GameClock* gameClock);
		~Hud();

		void update(int fps);
		
};


#endif // HUD_H_INCLUDED