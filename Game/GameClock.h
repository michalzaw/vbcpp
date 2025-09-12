#ifndef GAMECLOCK_H_INCLUDED
#define GAMECLOCK_H_INCLUDED


#include "../Utils/Time.h"
#include <chrono>
#include <string>


class GameClock final : public Time
{
	public:
		GameClock();
		~GameClock();

		void update(float deltaTime);

		void setTime(int hour = 0, int minutes = 0, int seconds = 0);

		GameClock& operator=(const Time& time);

};


#endif // GAMECLOCK_H_INCLUDED