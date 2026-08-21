#ifndef CURRENTROUTEDATA_H_INCLUDED
#define CURRENTROUTEDATA_H_INCLUDED


#include <vector>

#include "CurrentRouteBusStopsStatsData.h"
#include "Schedule.h"

#include "../../Utils/Time.h"


class TransitSystem;
class BusStopComponent;


class CurrentRouteData final
{
	private:
		int _currentLineIndex;
		int _currentBrigadeIndex;
		int _currentRouteIndex;

		ScheduleRoute* _currentRoute;

		int _currentBusStopIndex;
		int _nextBusStopIndex;

		std::vector<CurrentRouteBusStopsStatsData> _busStopsStatsData;

		TransitSystem* _transitSystem;

	public:
		CurrentRouteData(TransitSystem* transitSystem);

		LUAF bool isSet() const;

		LUAF void setRoute(int lineIndex, int brigadeIndex, int routeIndex);

		LUAF int getCurrentLineIndex() const;
		LUAF int getCurrentBrigadeIndex() const;
		LUAF int getCurrentRouteIndex() const;

		LUAF ScheduleRoute* getCurrentRoute() const;

		LUAF ScheduleStop* getBusStopScheduleData(int index) const;

		LUAF BusStopComponent* getBusStopOnRoute(int index) const;

		LUAF BusStopComponent* getCurrentBusStop() const;
		LUAF BusStopComponent* getNextBusStop() const;

		LUAF bool isCurrentBusStopSet() const;
		LUAF bool isNextBusStopSet() const;

		LUAF int getCurrentBusStopIndex() const;
		LUAF int getNextBusStopIndex() const;

		void enterToNextBusStop();
		void leaveCurrentBusStop();

		void setArrivalStatistics(const Time& time, int numberOfPassengersWhoWantedToGetOff, int numberOfPassengersWhoWantedToGetIn);
		void setDepartureStatistics(const Time& time, int numberOfPassengersWhoGotOff, int numberOfPassengersWhoGotIn);

		LUAF const std::vector<CurrentRouteBusStopsStatsData>& getBusStopsStatsData() const;

};


#endif // CURRENTROUTEDATA_H_INCLUDED
