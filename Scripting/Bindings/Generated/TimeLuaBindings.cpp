// Generated file
#include "TimeLuaBindings.h"

#include "../../../Utils/Time.h"


namespace TimeLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<Time>("Time",

            "getHour", &Time::getHour,
            "getMinutes", &Time::getMinutes,
            "getSeconds", &Time::getSeconds,
            "toString", &Time::toString



        );
    }
}
