#ifndef GAMECLOCK_H_INCLUDED
#define GAMECLOCK_H_INCLUDED


#include <chrono>
#include <string>


class GameClock final
{
	private:
		std::chrono::milliseconds _time;

	public:
		GameClock();
		~GameClock();

		void update(float deltaTime);

		int getHours();
		int getMinutes();
		int getSeconds();

		std::string getFormatedTime();

		void setTime(int hour = 0, int minute = 0, int seconds = 0);
		void setRandomTime();
		void setCurrentSystemTime();

};


#endif // GAMECLOCK_H_INCLUDED