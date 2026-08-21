// Generated file
#include "TransitSystemLuaBindings.h"

#include "../../../Game/Transit/BusRoutes.h"
#include "../../../Game/Transit/TransitSystem.h"


namespace TransitSystemLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<TransitSystem>("TransitSystem",

            "getCurrentBusStop", &TransitSystem::getCurrentBusStop,
            "getDistanceToCurrentBusStop", &TransitSystem::getDistanceToCurrentBusStop,
            "getBusStop", &TransitSystem::getBusStop,
            "getBusStopsCount", &TransitSystem::getBusStopsCount,
            "findBusStopById", &TransitSystem::findBusStopById,
            "getRoutes", &TransitSystem::getRoutes,
            "getSchedule", &TransitSystem::getSchedule,
            "getCurrentRouteData", &TransitSystem::getCurrentRouteData,
            "setCurrentRoute", &TransitSystem::setCurrentRoute,
            "setCurrentBusRoute", &TransitSystem::setCurrentBusRoute,
            "getCurrentBusRoute", &TransitSystem::getCurrentBusRoute



        );
    }
}
