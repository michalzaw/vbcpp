#ifndef CURRENTROUTEDATA_H_INCLUDED
#define CURRENTROUTEDATA_H_INCLUDED


#include <vector>

#include "Schedule.h"

#include "../../Utils/Time.h"


struct CurrentRouteBusStopsStatsData final
{
	bool isVisited;

	Time arrivalTime;
	Time departureTime;

	int numberOfPassengersWhoWantedToGetOff;
	int numberOfPassengersWhoGotOff;

	int numberOfPassengersWhoWantedToGetIn;
	int numberOfPassengersWhoGotIn;

	CurrentRouteBusStopsStatsData()
		: isVisited(false),
		numberOfPassengersWhoWantedToGetOff(0), numberOfPassengersWhoGotOff(0),
		numberOfPassengersWhoWantedToGetIn(0), numberOfPassengersWhoGotIn(0)
	{

	}

};


struct CurrentRouteData final
{
	int currentLineIndex;
	int currentBrigadeIndex;
	int currentRouteIndex;

	ScheduleRoute* currentRoute;

	int currentBusStopIndex;
	int nextBusStopIndex;

	std::vector<CurrentRouteBusStopsStatsData> busStopsStatsData;

	CurrentRouteData()
		: currentLineIndex(-1), currentBrigadeIndex(-1), currentRouteIndex(-1),
		currentRoute(nullptr),
		currentBusStopIndex(-1), nextBusStopIndex(-1)
	{

	}

	bool isSet() const
	{
		return currentLineIndex >= 0 && currentBrigadeIndex >= 0 && currentRouteIndex >= 0 && currentRoute != nullptr;
	}

};


#endif // CURRENTROUTEDATA_H_INCLUDED
