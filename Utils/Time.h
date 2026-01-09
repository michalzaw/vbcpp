#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED


#include <chrono>
#include <string>


class Time
{
	protected:
		std::chrono::milliseconds _time;

		void setTimeInternal(int hour = 0, int minutes = 0, int seconds = 0);

	public:
		Time(int hour = 0, int minutes = 0, int seconds = 0);
		Time(std::chrono::milliseconds time);

		int getHour() const;
		int getMinutes() const;
		int getSeconds() const;

		inline const std::chrono::milliseconds& getTime() const { return _time; }

		std::string toString() const;

		static Time randomTime();
		static Time currentSystemTime();

};


#endif // TIME_H_INCLUDED
