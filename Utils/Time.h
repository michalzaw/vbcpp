#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED


#include <chrono>
#include <string>

#include "../Scripting/Utils/LuaMacros.h"


class Time
{
	protected:
		std::chrono::milliseconds _time;

		void setTimeInternal(int hour = 0, int minutes = 0, int seconds = 0);

	public:
		LUAC Time(int hour = 0, int minutes = 0, int seconds = 0);
		Time(std::chrono::milliseconds time);

		LUAF int getHour() const;
		LUAF int getMinutes() const;
		LUAF int getSeconds() const;

		inline const std::chrono::milliseconds& getTime() const { return _time; }

		LUAF std::string toString() const;

		static Time randomTime();
		static Time currentSystemTime();

};


#endif // TIME_H_INCLUDED
