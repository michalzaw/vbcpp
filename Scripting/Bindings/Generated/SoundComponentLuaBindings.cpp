// Generated file
#include "SoundComponentLuaBindings.h"

#include "../../../Scene/SoundComponent.h"


namespace SoundComponentLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<SoundComponent>("SoundComponent",
            sol::base_classes, sol::bases<Component>(),

            "setPosition", &SoundComponent::setPosition,
            "getPosition", &SoundComponent::getPosition,
            "getPlayDistance", &SoundComponent::getPlayDistance,
            "setPlayDistance", &SoundComponent::setPlayDistance,
            "play", &SoundComponent::play,
            "stop", &SoundComponent::stop,
            "getState", &SoundComponent::getState,
            "setGain", &SoundComponent::setGain,
            "setPitch", &SoundComponent::setPitch,
            "setMute", &SoundComponent::setMute



        );
    }
}
