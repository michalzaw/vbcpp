#include "GameClock.h"

#include <iomanip>
#include <sstream>


GameClock::GameClock()
	: _time(0)
{
	
}


GameClock::~GameClock()
{

}


void GameClock::update(float deltaTime)
{
	auto deltaTimeDuration = std::chrono::duration<float>(deltaTime);

	_time += std::chrono::duration_cast<std::chrono::milliseconds>(deltaTimeDuration);
}


int GameClock::getHours()
{
	return std::chrono::duration_cast<std::chrono::hours>(_time).count();
}


int GameClock::getMinutes()
{
	return std::chrono::duration_cast<std::chrono::minutes>(_time).count() % 60;
}


int GameClock::getSeconds()
{
	return std::chrono::duration_cast<std::chrono::seconds>(_time).count() % 60;
}


std::string GameClock::getFormatedTime()
{
	std::ostringstream stream;
	stream << std::setw(2) << std::setfill('0') << getHours() << ":"
		   << std::setw(2) << std::setfill('0') << getMinutes() << ":"
		   << std::setw(2) << std::setfill('0') << getSeconds();

	return stream.str();
}


void GameClock::setTime(int hour/* = 0*/, int minute/* = 0*/, int seconds/* = 0*/)
{
	auto hoursDuration = std::chrono::duration<int>(hour * 3600);
	auto minutesDuration = std::chrono::duration<int>(minute* 60);
	auto secondDuration = std::chrono::duration<int>(seconds);

	_time = std::chrono::duration_cast<std::chrono::milliseconds>(hoursDuration) +
			std::chrono::duration_cast<std::chrono::milliseconds>(minutesDuration) +
			std::chrono::duration_cast<std::chrono::milliseconds>(secondDuration);
}


void GameClock::setRandomTime()
{
	setTime(rand() % 24, rand() % 60);
}


void GameClock::setCurrentSystemTime()
{
	auto now = std::chrono::system_clock::now();
	auto nowTime = std::chrono::system_clock::to_time_t(now);
	std::tm* localTime = std::localtime(&nowTime);

	int hour = localTime->tm_hour;
	int minute = localTime->tm_min;
	int second = localTime->tm_sec;

	setTime(hour, minute, second);
}
