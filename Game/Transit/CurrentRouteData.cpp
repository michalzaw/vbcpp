#include "CurrentRouteData.h"

#include "TransitSystem.h"


CurrentRouteData::CurrentRouteData(TransitSystem* transitSystem)
	: _currentLineIndex(-1), _currentBrigadeIndex(-1), _currentRouteIndex(-1),
	_currentRoute(nullptr),
	_currentBusStopIndex(-1), _nextBusStopIndex(-1),
	_transitSystem(transitSystem)
{

}

bool CurrentRouteData::isSet() const
{
	return _currentLineIndex >= 0 && _currentBrigadeIndex >= 0 && _currentRouteIndex >= 0 && _currentRoute != nullptr;
}


void CurrentRouteData::setRoute(int lineIndex, int brigadeIndex, int routeIndex)
{
    if (lineIndex < 0 || brigadeIndex < 0 || routeIndex < 0)
    {
        _currentLineIndex = -1;
        _currentBrigadeIndex = -1;
        _currentRouteIndex = -1;
        _currentRoute = nullptr;
        _currentBusStopIndex = -1;
        _nextBusStopIndex = -1;
    }
    else
    {
        Schedule* schedule = _transitSystem->getSchedule();
        if (lineIndex < schedule->lines.size() &&
            brigadeIndex < schedule->lines[lineIndex].brigades.size() &&
            routeIndex < schedule->lines[lineIndex].brigades[brigadeIndex].routes.size())
        {
            _currentLineIndex = lineIndex;
            _currentBrigadeIndex = brigadeIndex;
            _currentRouteIndex = routeIndex;
            _currentRoute = &(schedule->lines[lineIndex].brigades[brigadeIndex].routes[routeIndex]);
            _currentBusStopIndex = -1;
            _nextBusStopIndex = 0;

            _busStopsStatsData.clear();
            _busStopsStatsData.resize(schedule->lines[lineIndex].brigades[brigadeIndex].routes[routeIndex].stops.size());
        }
        else
        {
            LOG_ERROR("Invalid current route data: " + LOG_VARIABLE(lineIndex) + ", " + LOG_VARIABLE(brigadeIndex) + ", " + LOG_VARIABLE(routeIndex));
        }
    }
}


int CurrentRouteData::getCurrentLineIndex() const
{
    return _currentLineIndex;
}


int CurrentRouteData::getCurrentBrigadeIndex() const
{
    return _currentBrigadeIndex;
}


int CurrentRouteData::getCurrentRouteIndex() const
{
    return _currentRouteIndex;
}


ScheduleRoute* CurrentRouteData::getCurrentRoute() const
{
    return _currentRoute;
}


ScheduleStop* CurrentRouteData::getBusStopScheduleData(int index) const
{
    if (isSet() && index >= 0 && index < _currentRoute->stops.size())
    {
        return &(_currentRoute->stops[index]);
    }

    return nullptr;
}


BusStopComponent* CurrentRouteData::getBusStopOnRoute(int index) const
{
    if (isSet() && index >= 0 && index < _currentRoute->stops.size())
    {
        return _transitSystem->findBusStopById(_currentRoute->stops[index].id);
    }

    return nullptr;
}


BusStopComponent* CurrentRouteData::getCurrentBusStop() const
{
	if (isSet() && _currentBusStopIndex >= 0)
	{
		return _transitSystem->findBusStopById(_currentRoute->stops[_currentBusStopIndex].id);
	}
	
	return nullptr;
}


BusStopComponent* CurrentRouteData::getNextBusStop() const
{
    if (isSet() && _nextBusStopIndex >= 0)
    {
        return _transitSystem->findBusStopById(_currentRoute->stops[_nextBusStopIndex].id);
    }

    return nullptr;
}


bool CurrentRouteData::isCurrentBusStopSet() const
{
    return _currentBusStopIndex >= 0;
}


bool CurrentRouteData::isNextBusStopSet() const
{
    return _nextBusStopIndex >= 0;
}


int CurrentRouteData::getCurrentBusStopIndex() const
{
    return _currentBusStopIndex;
}


int CurrentRouteData::getNextBusStopIndex() const
{
    return _nextBusStopIndex;
}


void CurrentRouteData::enterToNextBusStop()
{
    _currentBusStopIndex = _nextBusStopIndex;
    _nextBusStopIndex += 1;

    if (_nextBusStopIndex >= _currentRoute->stops.size())
    {
        // koniec trasy
        _nextBusStopIndex = -1;
    }
}


void CurrentRouteData::leaveCurrentBusStop()
{
    _currentBusStopIndex = -1;
}


void CurrentRouteData::setArrivalStatistics(const Time& time, int numberOfPassengersWhoWantedToGetOff, int numberOfPassengersWhoWantedToGetIn)
{
    _busStopsStatsData[_currentBusStopIndex].isVisited = true;
    _busStopsStatsData[_currentBusStopIndex].arrivalTime = time;
    _busStopsStatsData[_currentBusStopIndex].numberOfPassengersWhoWantedToGetOff = numberOfPassengersWhoWantedToGetOff;
    _busStopsStatsData[_currentBusStopIndex].numberOfPassengersWhoWantedToGetIn = numberOfPassengersWhoWantedToGetIn;
}


void CurrentRouteData::setDepartureStatistics(const Time& time, int numberOfPassengersWhoGotOff, int numberOfPassengersWhoGotIn)
{
    _busStopsStatsData[_currentBusStopIndex].departureTime = time;
    _busStopsStatsData[_currentBusStopIndex].numberOfPassengersWhoGotOff = numberOfPassengersWhoGotOff;
    _busStopsStatsData[_currentBusStopIndex].numberOfPassengersWhoGotIn = numberOfPassengersWhoGotIn;
}


const std::vector<CurrentRouteBusStopsStatsData>& CurrentRouteData::getBusStopsStatsData() const
{
    return _busStopsStatsData;
}
