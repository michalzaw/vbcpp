// Generated file
#include "GameClockLuaBindings.h"

#include "../../../Game/GameClock.h"


namespace GameClockLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<GameClock>("GameClock",
            sol::base_classes, sol::bases<Time>(),

            "setTime", &GameClock::setTime



        );
    }
}
