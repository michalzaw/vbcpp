// Generated file
#include "ScheduleLuaBindings.h"

#include "../../../Game/Transit/Schedule.h"


namespace ScheduleLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<ScheduleStop>("ScheduleStop",
            "id", &ScheduleStop::id,
            "time", &ScheduleStop::time
        );
        
        lua->new_usertype<ScheduleRoute>("ScheduleRoute",
            "direction", &ScheduleRoute::direction,
            "startTime", &ScheduleRoute::startTime,
            "stops", &ScheduleRoute::stops
        );
        
        lua->new_usertype<ScheduleBrigade>("ScheduleBrigade",
            "id", &ScheduleBrigade::id,
            "name", &ScheduleBrigade::name,
            "routes", &ScheduleBrigade::routes
        );
        
        lua->new_usertype<ScheduleLine>("ScheduleLine",
            "id", &ScheduleLine::id,
            "number", &ScheduleLine::number,
            "brigades", &ScheduleLine::brigades
        );

        lua->new_usertype<Schedule>("Schedule",
            "fileName", &Schedule::fileName,
            "description", &Schedule::description,
            "lines", &Schedule::lines
        );
    }
}
