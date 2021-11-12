#ifndef BUSSTOPSYSTEM_H_INCLUDED
#define BUSSTOPSYSTEM_H_INCLUDED


#include <vector>
#include <cstdlib>
#include <memory>

#include "BusStopComponent.h"

#include "../Bus/Bus.h"


class BusStopSystem
{
    private:
        static const int MAX_NUMBER_OF_PASSENGERS = 30;
        static constexpr float MIN_DISTANCE_TO_BUS_STOP = 10.0f;

        std::vector<BusStopComponent*> _busStops;

        BusStopComponent* _currentBusStop;
        float _distanceToCurrentBusStop;

    public:
        BusStopSystem()
        {
            _currentBusStop = NULL;
            _distanceToCurrentBusStop = 0.0f;
        }

        ~BusStopSystem()
        {
            for (std::vector<BusStopComponent*>::iterator i = _busStops.begin(); i != _busStops.end(); ++i)
            {
                delete *i;
            }
        }

        BusStopComponent* addBusStopComponent(std::string name)
        {
            BusStopComponent* busStop = new BusStopComponent(name);
            busStop->setNumberOfPassengers(rand() % MAX_NUMBER_OF_PASSENGERS);

            _busStops.push_back(busStop);

            return busStop;
        }

        void removeBusStop(BusStopComponent* busStop)
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

        BusStopComponent* getCurrentBusStop()
        {
            return _currentBusStop;
        }

        float getDistanceToCurrentBusStop()
        {
            return _distanceToCurrentBusStop;
        }

        BusStopComponent* getBusStop(int index)
        {
            return _busStops[index];
        }

        int getBusStopsCount()
        {
            return _busStops.size();
        }

        void update(float deltaTime, Bus* bus)
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

};


#endif // BUSSTOPSYSTEM_H_INCLUDED
