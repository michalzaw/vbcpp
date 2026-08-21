#ifndef CURRENTROUTEBUSSTOPSTATSDATA_H_INCLUDED
#define CURRENTROUTEBUSSTOPSTATSDATA_H_INCLUDED


#include "../../Scripting/Utils/LuaMacros.h"

#include "../../Utils/Time.h"


struct CurrentRouteBusStopsStatsData final
{
	LUAV bool isVisited;

	LUAV Time arrivalTime;
	LUAV Time departureTime;

	LUAV int numberOfPassengersWhoWantedToGetOff;
	LUAV int numberOfPassengersWhoGotOff;

	LUAV int numberOfPassengersWhoWantedToGetIn;
	LUAV int numberOfPassengersWhoGotIn;

	CurrentRouteBusStopsStatsData()
		: isVisited(false),
		numberOfPassengersWhoWantedToGetOff(0), numberOfPassengersWhoGotOff(0),
		numberOfPassengersWhoWantedToGetIn(0), numberOfPassengersWhoGotIn(0)
	{

	}

};


#endif // CURRENTROUTEBUSSTOPSTATSDATA_H_INCLUDED
