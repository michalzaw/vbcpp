#include "TransitSystem.h"


TransitSystem::TransitSystem()
    : _currentBusStop(nullptr), _distanceToCurrentBusStop(0.0f)
{

}


TransitSystem::~TransitSystem()
{
    for (std::vector<BusStopComponent*>::iterator i = _busStops.begin(); i != _busStops.end(); ++i)
    {
        delete* i;
    }
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


void TransitSystem::update(float deltaTime, Bus* bus)
{
    BusStopComponent* nearestBusStop = NULL;
    float minDistance = FLT_MAX;
    for (int i = 0; i < _busStops.size(); ++i)
    {
        float distance = glm::length(bus->getSceneObject()->getPosition() - _busStops[i]->getSceneObject()->getPosition());
        if (distance < minDistance)
        {
            minDistance = distance;
            nearestBusStop = _busStops[i];
        }

        if (distance < 50.0f && !_busStops[i]->getAnnouncementIsPlay())
        {
            bus->getAnnouncementSource()->setSound(_busStops[i]->getAnnouncementSound());
            bus->getAnnouncementSource()->play();

            _busStops[i]->setAnnouncementIsPlay(true);
        }
    }

    if (minDistance < MIN_DISTANCE_TO_BUS_STOP)
    {
        _distanceToCurrentBusStop = minDistance;

        if (_currentBusStop != NULL && _currentBusStop != nearestBusStop)
        {
            _currentBusStop->_time = 0.0f;
        }
        _currentBusStop = nearestBusStop;
        _currentBusStop->onTrigger(deltaTime, bus);
    }
    else
    {
        _currentBusStop = NULL;
    }
}
