// Generated file
#include "AnimationStateLuaBindings.h"

#include "../../../Graphics/AnimationState.h"


namespace AnimationStateLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<AnimationState>("AnimationState",

            "setCurrentTime", &AnimationState::setCurrentTime,
            "getCurrentTime", &AnimationState::getCurrentTime,



            "name", &AnimationState::name,
            "currentTime", &AnimationState::currentTime,
            "previousTime", &AnimationState::previousTime,
            "rootLoopDeltaPosition", &AnimationState::rootLoopDeltaPosition,
            "previousRootPosition", &AnimationState::previousRootPosition,
            "rootDeltaInLastFrame", &AnimationState::rootDeltaInLastFrame
        );
    }
}
