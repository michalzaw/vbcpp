// Generated file
#include "AIAgentLuaBindings.h"

#include "../../../Game/AI/AIAgent.h"


namespace AIAgentLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<AIAgent>("AIAgent",
            sol::base_classes, sol::bases<Component>(),

            "setSpeed", &AIAgent::setSpeed,
            "getSpeed", &AIAgent::getSpeed,
            "setMotionFromAnimation", &AIAgent::setMotionFromAnimation,
            "isMotionFromAnimation", &AIAgent::isMotionFromAnimation,
            "setCurrentPath", &AIAgent::setCurrentPath,
            "getCurrentPath", &AIAgent::getCurrentPath



        );
    }
}
