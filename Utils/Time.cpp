#include "Time.h"

#include <sstream>
#include <iomanip>

#include "Logger.h"


Time::Time(int hour, int minutes, int seconds/* = 0*/)
{
	setTimeInternal(hour, minutes, seconds);
}


Time::Time(std::chrono::milliseconds time)
	: _time(time)
{

}


void Time::setTimeInternal(int hour/* = 0*/, int minutes/* = 0*/, int seconds/* = 0*/)
{
	if (hour > 23)
	{
		LOG_ERROR("Invalid hour value " + Strings::toString(hour));
		hour = 0;
	}

	if (minutes > 59)
	{
		LOG_ERROR("Invalid minutes value " + Strings::toString(minutes));
		minutes = 0;
	}

	if (seconds > 59)
	{
		LOG_ERROR("Invalid seconds value " + Strings::toString(seconds));
		seconds = 0;
	}

	auto hoursDuration = std::chrono::duration<int>(hour * 3600);
	auto minutesDuration = std::chrono::duration<int>(minutes* 60);
	auto secondDuration = std::chrono::duration<int>(seconds);

	_time = std::chrono::duration_cast<std::chrono::milliseconds>(hoursDuration) +
			std::chrono::duration_cast<std::chrono::milliseconds>(minutesDuration) +
			std::chrono::duration_cast<std::chrono::milliseconds>(secondDuration);
}


int Time::getHour() const
{
	return std::chrono::duration_cast<std::chrono::hours>(_time).count();
}


int Time::getMinutes() const
{
	return std::chrono::duration_cast<std::chrono::minutes>(_time).count() % 60;
}


int Time::getSeconds() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(_time).count() % 60;
}


std::string Time::toString() const
{
	std::ostringstream stream;
	stream << std::setw(2) << std::setfill('0') << getHour() << ":"
		<< std::setw(2) << std::setfill('0') << getMinutes() << ":"
		<< std::setw(2) << std::setfill('0') << getSeconds();

	return stream.str();
}


Time Time::randomTime()
{
	return Time(rand() % 24, rand() % 60);
}


Time Time::currentSystemTime()
{
	auto now = std::chrono::system_clock::now();
	auto nowTime = std::chrono::system_clock::to_time_t(now);
	std::tm* localTime = std::localtime(&nowTime);

	int hour = localTime->tm_hour;
	int minute = localTime->tm_min;
	int second = localTime->tm_sec;

	return Time(hour, minute, second);

	/*auto now = std::chrono::system_clock::now();
	return Time(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()));*/
}
