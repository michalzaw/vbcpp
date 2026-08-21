// Generated file
#include "CurrentRouteBusStopsStatsDataLuaBindings.h"

#include "../../../Game/Transit/CurrentRouteBusStopsStatsData.h"


namespace CurrentRouteBusStopsStatsDataLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<CurrentRouteBusStopsStatsData>("CurrentRouteBusStopsStatsData",




            "isVisited", &CurrentRouteBusStopsStatsData::isVisited,
            "arrivalTime", &CurrentRouteBusStopsStatsData::arrivalTime,
            "departureTime", &CurrentRouteBusStopsStatsData::departureTime,
            "numberOfPassengersWhoWantedToGetOff", &CurrentRouteBusStopsStatsData::numberOfPassengersWhoWantedToGetOff,
            "numberOfPassengersWhoGotOff", &CurrentRouteBusStopsStatsData::numberOfPassengersWhoGotOff,
            "numberOfPassengersWhoWantedToGetIn", &CurrentRouteBusStopsStatsData::numberOfPassengersWhoWantedToGetIn,
            "numberOfPassengersWhoGotIn", &CurrentRouteBusStopsStatsData::numberOfPassengersWhoGotIn
        );
    }
}
