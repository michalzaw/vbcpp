// Generated file
#include "EngineLuaBindings.h"

#include "../../../Utils/Engine.h"


namespace EngineLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<Engine>("Engine",

            "getThrottle", &Engine::getThrottle,
            "getCurrentRPM", &Engine::getCurrentRPM,
            "isRunning", &Engine::isRunning,
            "getCurrentTorque", &Engine::getCurrentTorque,
            "getDifferentialRatio", &Engine::getDifferentialRatio



        );
    }
}
