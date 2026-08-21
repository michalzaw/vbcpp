#include "TransitSystem.h"

#include "BusRoutes.h"
#include "Schedule.h"

#include "../GameClock.h"
#include "../GameLogicSystem.h"

#include "../../Scene/SceneManager.h"


TransitSystem::TransitSystem()
    : _distanceToCurrentBusStop(0.0f),
    _currentRouteData(this)
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
    _currentRouteData.setRoute(lineIndex, brigadeIndex, routeIndex);
}


void TransitSystem::setCurrentBusRoute(BusRoute* busRoute)
{
    _currentBusRoute = busRoute;
}


BusRoute* TransitSystem::getCurrentBusRoute()
{
    return _currentBusRoute;
}


void TransitSystem::update(float deltaTime, Bus* bus)
{
    BusStopComponent* nextBusStop = _currentRouteData.getNextBusStop();

    if (_currentRouteData.isSet() && nextBusStop != nullptr)
    {
        float distance = glm::length(bus->getSceneObject()->getPosition() - nextBusStop->getSceneObject()->getPosition());

        // Zapowiedz
        if (distance < 50.0f && !nextBusStop->getAnnouncementIsPlay())
        {
            bus->getAnnouncementSource()->setSound(nextBusStop->getAnnouncementSound());
            bus->getAnnouncementSource()->play();

            nextBusStop->setAnnouncementIsPlay(true);
        }

        // Przyjazd
        if (distance < MIN_DISTANCE_TO_BUS_STOP && !_currentRouteData.isCurrentBusStopSet())
        {
            _currentRouteData.enterToNextBusStop();

            BusStopComponent* currentBusStop = _currentRouteData.getCurrentBusStop();

            currentBusStop->_time = 0.0f;

            Time* currentTime = currentBusStop->getSceneObject()->getSceneManager()->getGameLogicSystem()->getGameClock();
            int numberOfPassengersWhoWantedToGetOff = 0; // todo

            _currentRouteData.setArrivalStatistics(*currentTime, numberOfPassengersWhoWantedToGetOff, currentBusStop->getNumberOfPassengers());
        }
    }

    // Obecny przystanek
    BusStopComponent* currentBusStop = _currentRouteData.getCurrentBusStop();

    if (currentBusStop != nullptr)
    {
        currentBusStop->onTrigger(deltaTime, bus);

        _distanceToCurrentBusStop = glm::length(bus->getSceneObject()->getPosition() - currentBusStop->getSceneObject()->getPosition());

        // Odjazd
        if (_distanceToCurrentBusStop >= MIN_DISTANCE_TO_BUS_STOP)
        {
            Time* currentTime = currentBusStop->getSceneObject()->getSceneManager()->getGameLogicSystem()->getGameClock();
            int numberOfPassengersWhoGotOff = 0; // todo
            int numberOfPassengersWhoGotIn = _currentRouteData.getBusStopsStatsData()[_currentRouteData.getCurrentBusStopIndex()].numberOfPassengersWhoWantedToGetIn - currentBusStop->getNumberOfPassengers();

            _currentRouteData.setDepartureStatistics(*currentTime, numberOfPassengersWhoGotOff, numberOfPassengersWhoGotIn);

            _currentRouteData.leaveCurrentBusStop();
        }
    }
}
