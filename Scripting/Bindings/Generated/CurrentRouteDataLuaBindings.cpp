// Generated file
#include "CurrentRouteDataLuaBindings.h"

#include "../../../Game/Transit/BusStopComponent.h"
#include "../../../Game/Transit/CurrentRouteData.h"


namespace CurrentRouteDataLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<CurrentRouteData>("CurrentRouteData",

            "isSet", &CurrentRouteData::isSet,
            "setRoute", &CurrentRouteData::setRoute,
            "getCurrentLineIndex", &CurrentRouteData::getCurrentLineIndex,
            "getCurrentBrigadeIndex", &CurrentRouteData::getCurrentBrigadeIndex,
            "getCurrentRouteIndex", &CurrentRouteData::getCurrentRouteIndex,
            "getCurrentRoute", &CurrentRouteData::getCurrentRoute,
            "getBusStopScheduleData", &CurrentRouteData::getBusStopScheduleData,
            "getBusStopOnRoute", &CurrentRouteData::getBusStopOnRoute,
            "getCurrentBusStop", &CurrentRouteData::getCurrentBusStop,
            "getNextBusStop", &CurrentRouteData::getNextBusStop,
            "isCurrentBusStopSet", &CurrentRouteData::isCurrentBusStopSet,
            "isNextBusStopSet", &CurrentRouteData::isNextBusStopSet,
            "getCurrentBusStopIndex", &CurrentRouteData::getCurrentBusStopIndex,
            "getNextBusStopIndex", &CurrentRouteData::getNextBusStopIndex,
            "getBusStopsStatsData", &CurrentRouteData::getBusStopsStatsData



        );
    }
}
