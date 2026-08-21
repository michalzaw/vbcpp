// Generated file
#include "BusStopComponentLuaBindings.h"

#include "../../../Game/Transit/BusStopComponent.h"


namespace BusStopComponentLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<BusStopComponent>("BusStopComponent",
            sol::base_classes, sol::bases<Component>(),

            "setNumberOfPassengers", &BusStopComponent::setNumberOfPassengers,
            "getNumberOfPassengers", &BusStopComponent::getNumberOfPassengers,
            "setName", &BusStopComponent::setName,
            "getName", &BusStopComponent::getName,
            "setId", &BusStopComponent::setId,
            "getId", &BusStopComponent::getId,
            "setAnnouncementFileName", &BusStopComponent::setAnnouncementFileName,
            "getAnnouncementFileName", &BusStopComponent::getAnnouncementFileName,
            "setDistance", &BusStopComponent::setDistance,
            "getDistance", &BusStopComponent::getDistance,
            "setRequestStop", &BusStopComponent::setRequestStop,
            "isRequestStop", &BusStopComponent::isRequestStop,
            "setNumberOfPassengersMin", &BusStopComponent::setNumberOfPassengersMin,
            "getNumberOfPassengersMin", &BusStopComponent::getNumberOfPassengersMin,
            "setNumberOfPassengersMax", &BusStopComponent::setNumberOfPassengersMax,
            "getNumberOfPassengersMax", &BusStopComponent::getNumberOfPassengersMax,
            "getAnnouncementIsPlay", &BusStopComponent::getAnnouncementIsPlay,
            "setAnnouncementIsPlay", &BusStopComponent::setAnnouncementIsPlay



        );
    }
}
