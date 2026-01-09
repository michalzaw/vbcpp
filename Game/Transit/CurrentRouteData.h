#ifndef CURRENTROUTEDATA_H_INCLUDED
#define CURRENTROUTEDATA_H_INCLUDED


#include <vector>

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

};


struct CurrentRouteData final
{
	int currentLineIndex;
	int currentBrigadeIndex;
	int currentRouteIndex;

	std::vector<CurrentRouteBusStopsStatsData> busStopsStatsData;

	CurrentRouteData()
		: currentLineIndex(-1), currentBrigadeIndex(-1), currentRouteIndex(-1)
	{

	}

	bool isSet() const
	{
		return currentLineIndex >= 0 && currentBrigadeIndex >= 0 && currentRouteIndex >= 0;
	}

};


#endif // CURRENTROUTEDATA_H_INCLUDED
