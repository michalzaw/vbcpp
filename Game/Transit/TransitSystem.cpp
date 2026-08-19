#include "TransitSystem.h"

#include "BusRoutes.h"
#include "Schedule.h"


TransitSystem::TransitSystem()
    : _distanceToCurrentBusStop(0.0f)
{
    _routes = new BusRoutes;
    _schedule = new Schedule;
}


TransitSystem::~TransitSystem()
{
    for (std::vector<BusStopComponent*>::iterator i = _busStops.begin(); i != _busStops.end(); ++i)
    {
        delete* i;
    }

    delete _routes;
    delete _schedule;
}


BusStopComponent* TransitSystem::addBusStopComponent(const std::string& name)
{
    BusStopComponent* busStop = new BusStopComponent(name);
    busStop->setNumberOfPassengers(rand() % MAX_NUMBER_OF_PASSENGERS);

    _busStops.push_back(busStop);

    return busStop;
}


void TransitSystem::removeBusStop(BusStopComponent* busStop)
{
    for (std::vector<BusStopComponent*>::iterator i = _busStops.begin(); i != _busStops.end(); ++i)
    {
        if (*i == busStop)
        {
            i = _busStops.erase(i);

            delete busStop;

            return;
        }
    }
}


BusStopComponent* TransitSystem::findBusStopById(int id)
{
    for (BusStopComponent* busStop : _busStops)
    {
        if (busStop->getId() == id)
        {
            return busStop;
        }
    }

    return nullptr;
}


void TransitSystem::setRoutes(BusRoutes* routes)
{
    if (_routes != nullptr)
    {
        delete _routes;
    }

    _routes = routes;
}


void TransitSystem::setSchedule(Schedule* schedule)
{
    if (_schedule != nullptr)
    {
        delete _schedule;
    }

    _schedule = schedule;
}


void TransitSystem::setCurrentRoute(int lineIndex, int brigadeIndex, int routeIndex)
{
    if (lineIndex < 0 || brigadeIndex < 0 || routeIndex < 0)
    {
        _currentRouteData.currentLineIndex = -1;
        _currentRouteData.currentBrigadeIndex = -1;
        _currentRouteData.currentRouteIndex = -1;
        _currentRouteData.currentRoute = nullptr;
        _currentRouteData.currentBusStopIndex = -1;
        _currentRouteData.nextBusStopIndex = -1;
    }
    else
    {
        if (lineIndex < _schedule->lines.size() && brigadeIndex < _schedule->lines[lineIndex].brigades.size() && routeIndex < _schedule->lines[lineIndex].brigades[brigadeIndex].routes.size())
        {
            _currentRouteData.currentLineIndex = lineIndex;
            _currentRouteData.currentBrigadeIndex = brigadeIndex;
            _currentRouteData.currentRouteIndex = routeIndex;
            _currentRouteData.currentRoute = &(_schedule->lines[lineIndex].brigades[brigadeIndex].routes[routeIndex]);
            _currentRouteData.currentBusStopIndex = -1;
            _currentRouteData.nextBusStopIndex = 0;

            _currentRouteData.busStopsStatsData.clear();
            _currentRouteData.busStopsStatsData.resize(_schedule->lines[lineIndex].brigades[brigadeIndex].routes[routeIndex].stops.size());
        }
        else
        {
            LOG_ERROR("Invalid current route data: " + LOG_VARIABLE(lineIndex) + ", " + LOG_VARIABLE(brigadeIndex) + ", " + LOG_VARIABLE(routeIndex));
        }
    }

}


void TransitSystem::update(float deltaTime, Bus* bus)
{
    if (_currentRouteData.isSet() && _currentRouteData.nextBusStopIndex >= 0)
    {
        BusStopComponent* nextBusStop = findBusStopById(_currentRouteData.currentRoute->stops[_currentRouteData.nextBusStopIndex].id);
        float distance = glm::length(bus->getSceneObject()->getPosition() - nextBusStop->getSceneObject()->getPosition());

        if (distance < 50.0f && !nextBusStop->getAnnouncementIsPlay())
        {
            bus->getAnnouncementSource()->setSound(nextBusStop->getAnnouncementSound());
            bus->getAnnouncementSource()->play();

            nextBusStop->setAnnouncementIsPlay(true);
        }

        if (distance < MIN_DISTANCE_TO_BUS_STOP && _currentRouteData.currentBusStopIndex < 0)
        {
            _currentRouteData.currentBusStopIndex = _currentRouteData.nextBusStopIndex;
            _currentRouteData.nextBusStopIndex += 1;
            if (_currentRouteData.nextBusStopIndex >= _currentRouteData.currentRoute->stops.size())
            {
                // koniec trasy
                _currentRouteData.nextBusStopIndex = -1;
            }

            BusStopComponent* currentBusStop = findBusStopById(_currentRouteData.currentRoute->stops[_currentRouteData.currentBusStopIndex].id);

            currentBusStop->_time = 0.0f;
        }

        if (_currentRouteData.currentBusStopIndex >= 0)
        {
            BusStopComponent* currentBusStop = findBusStopById(_currentRouteData.currentRoute->stops[_currentRouteData.currentBusStopIndex].id);

            currentBusStop->onTrigger(deltaTime, bus);

            _distanceToCurrentBusStop = glm::length(bus->getSceneObject()->getPosition() - currentBusStop->getSceneObject()->getPosition());

            if (_distanceToCurrentBusStop >= MIN_DISTANCE_TO_BUS_STOP)
            {
                _currentRouteData.currentBusStopIndex = -1;
            }
        }
    }
}
