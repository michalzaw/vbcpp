#ifndef SCHEDULELOADER_H_INCLUDED
#define SCHEDULELOADER_H_INCLUDED


#include "Schedule.h"


namespace ScheduleLoader
{
	Schedule* loadSchedule(const std::string& fileName);
}


#endif // SCHEDULELOADER_H_INCLUDED
