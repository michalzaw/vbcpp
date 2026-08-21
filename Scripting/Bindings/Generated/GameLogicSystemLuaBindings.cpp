// Generated file
#include "GameLogicSystemLuaBindings.h"

#include "../../../Game/GameLogicSystem.h"


namespace GameLogicSystemLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<GameLogicSystem>("GameLogicSystem",

            "getGameClock", &GameLogicSystem::getGameClock



        );
    }
}
