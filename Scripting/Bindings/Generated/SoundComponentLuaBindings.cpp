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
            "getGlobalPosition", &SoundComponent::getGlobalPosition,
            "getPlayDistance", &SoundComponent::getPlayDistance,
            "setPlayDistance", &SoundComponent::setPlayDistance,
            "play", &SoundComponent::play,
            "stop", &SoundComponent::stop,
            "getState", &SoundComponent::getState,
            "setGain", &SoundComponent::setGain,
            "getGain", &SoundComponent::getGain,
            "setPitch", &SoundComponent::setPitch,
            "getPitch", &SoundComponent::getPitch,
            "setMute", &SoundComponent::setMute,
            "setLooping", &SoundComponent::setLooping,
            "isLooping", &SoundComponent::isLooping



        );
    }
}
