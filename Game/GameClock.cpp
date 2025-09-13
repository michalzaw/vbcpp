#include "GameClock.h"

#include <iomanip>
#include <sstream>


GameClock::GameClock()
	: Time(std::chrono::milliseconds(0))
{
	
}


GameClock::~GameClock()
{

}


TimePoint GameClock::getCurrentTime()
{
	auto now = std::chrono::system_clock::now();

	return std::chrono::time_point_cast<std::chrono::milliseconds>(now);
}


void GameClock::update(float deltaTime)
{
	static TimePoint timePrevious = getCurrentTime();

	TimePoint timeCurrent = getCurrentTime();

	auto deltaTimeInMs = timeCurrent - timePrevious;

	timePrevious = timeCurrent;

	_time += std::chrono::duration_cast<std::chrono::milliseconds>(deltaTimeInMs);
}


void GameClock::setTime(int hour/* = 0*/, int minutes/* = 0*/, int seconds/* = 0*/)
{
	setTimeInternal(hour, minutes, seconds);
}


GameClock& GameClock::operator=(const Time& time)
{
	_time = time.getTime();

	return *this;
}
