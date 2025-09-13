#ifndef SCHEDULES_H_INCLUDED
#define SCHEDULES_H_INCLUDED


#include <string>
#include <vector>

#include "../../Utils/ResourceDescription.h"
#include "../../Utils/Time.h"


struct ScheduleStop final
{
	int id;
	Time time;

	ScheduleStop(int id, const Time& time)
		: id(id), time(time)
	{

	}

};


struct ScheduleRoute final
{
	std::string direction;
	Time startTime;

	std::vector<ScheduleStop> stops;

	ScheduleRoute(const std::string& direction, const Time& startTime)
		: direction(direction), startTime(startTime)
	{

	}

	std::string toString() const
	{
		return direction + " [" + startTime.toString() +"]";
	}

};


struct ScheduleBrigade final
{
	std::string id;
	std::string name;

	std::vector<ScheduleRoute> routes;

	ScheduleBrigade(const std::string& id, const std::string& name)
		: id(id), name(name)
	{

	}

	std::string toString() const
	{
		return id + " [" + name + "]";
	}

};


struct ScheduleLine final
{
	std::string id;
	std::string number;

	std::vector<ScheduleBrigade> brigades;

	ScheduleLine(const std::string& id, const std::string& number)
		: id(id), number(number)
	{

	}

	std::string toString() const
	{
		return id + " [" + number + "]";
	}

};


struct Schedule final
{
	ResourceDescription description;

	std::vector<ScheduleLine> lines;

};


#endif // SCHEDULES_H_INCLUDED
