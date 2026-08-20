#ifndef CURRENTROUTEDATA_H_INCLUDED
#define CURRENTROUTEDATA_H_INCLUDED


#include <vector>

#include "Schedule.h"

#include "../../Utils/Time.h"


class TransitSystem;
class BusStopComponent;


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

		bool isSet() const;

		void setRoute(int lineIndex, int brigadeIndex, int routeIndex);

		int getCurrentLineIndex() const;
		int getCurrentBrigadeIndex() const;
		int getCurrentRouteIndex() const;

		ScheduleRoute* getCurrentRoute() const;

		ScheduleStop* getBusStopScheduleData(int index) const;

		BusStopComponent* getBusStopOnRoute(int index) const;

		BusStopComponent* getCurrentBusStop() const;
		BusStopComponent* getNextBusStop() const;

		bool isCurrentBusStopSet() const;
		bool isNextBusStopSet() const;

		int getCurrentBusStopIndex() const;
		int getNextBusStopIndex() const;

		void enterToNextBusStop();
		void leaveCurrentBusStop();

		void setArrivalStatistics(const Time& time, int numberOfPassengersWhoWantedToGetOff, int numberOfPassengersWhoWantedToGetIn);
		void setDepartureStatistics(const Time& time, int numberOfPassengersWhoGotOff, int numberOfPassengersWhoGotIn);

		const std::vector<CurrentRouteBusStopsStatsData>& getBusStopsStatsData() const;

};


#endif // CURRENTROUTEDATA_H_INCLUDED
