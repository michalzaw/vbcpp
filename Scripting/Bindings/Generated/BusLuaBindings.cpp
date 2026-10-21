// Generated file
#include "BusLuaBindings.h"

#include "../../../Bus/Bus.h"


namespace BusLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<Bus>("Bus",
            sol::base_classes, sol::bases<RefCounter>(),

            "getSteeringWheelObject", &Bus::getSteeringWheelObject,
            "setIsEnableLights", &Bus::setIsEnableLights,
            "isEnableLights", &Bus::isEnableLights,
            "setIsEnableHeadlights", &Bus::setIsEnableHeadlights,
            "isEnableHeadlights", &Bus::isEnableHeadlights,
            "turnLeft", &Bus::turnLeft,
            "turnRight", &Bus::turnRight,
            "centerSteringWheel", &Bus::centerSteringWheel,
            "accelerate", &Bus::accelerate,
            "idle", &Bus::idle,
            "brakeOn", &Bus::brakeOn,
            "brakeOff", &Bus::brakeOff,
            "toggleHandbrake", &Bus::toggleHandbrake,
            "getHandbrakeState", &Bus::getHandbrakeState,
            "getGearbox", &Bus::getGearbox,
            "getEngine", &Bus::getEngine,
            "startEngine", &Bus::startEngine,
            "stopEngine", &Bus::stopEngine,
            "doorGroupOpenClose", [](Bus* self, int groupIndex) { self->doorGroupOpenClose(groupIndex); },
            "getDoorsCount", &Bus::getDoorsCount,
            "getMirror", &Bus::getMirror,
            "getMirrorsCount", &Bus::getMirrorsCount,
            "getDisplayText", &Bus::getDisplayText,
            "updateDisplays", &Bus::updateDisplays,
            "getBusSpeed", &Bus::getBusSpeed,
            "getNumberOfPassengers", &Bus::getNumberOfPassengers,
            "getNumberOfPassengersGettingOff", &Bus::getNumberOfPassengersGettingOff,
            "getAnnouncementSource", &Bus::getAnnouncementSource,
            "getEngineSoundSource", &Bus::getEngineSoundSource,
            "getEngineStartSoundSource", &Bus::getEngineStartSoundSource,
            "getEngineStopSoundSource", &Bus::getEngineStopSoundSource,
            "getText", &Bus::getText



        );
    }
}
