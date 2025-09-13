#include "GameClock.h"

#include <iomanip>
#include <sstream>


typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> TimePoint;


GameClock::GameClock()
	: Time(std::chrono::milliseconds(0))
{
	
}


GameClock::~GameClock()
{

}


void GameClock::update(float deltaTime)
{
	auto now = std::chrono::system_clock::now();
	auto nowMs = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

	static auto lastTime = nowMs;

	auto delta = nowMs - lastTime;

	lastTime = nowMs;

	auto ms = nowMs.time_since_epoch().count();

	auto deltaTimeDuration = std::chrono::duration<float>(deltaTime);

	_time += std::chrono::duration_cast<std::chrono::milliseconds>(delta);
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
