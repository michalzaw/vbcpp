#ifndef TRANSITSYSTEM_H_INCLUDED
#define TRANSITSYSTEM_H_INCLUDED


#include <vector>
#include <cstdlib>
#include <memory>

#include "BusStopComponent.h"

#include "../../Bus/Bus.h"


class TransitSystem
{
    private:
        static const int MAX_NUMBER_OF_PASSENGERS = 30;
        static constexpr float MIN_DISTANCE_TO_BUS_STOP = 10.0f;

        std::vector<BusStopComponent*> _busStops;

        BusStopComponent* _currentBusStop;
        float _distanceToCurrentBusStop;

    public:
        TransitSystem();
        ~TransitSystem();

        BusStopComponent* addBusStopComponent(const std::string& name);

        void removeBusStop(BusStopComponent* busStop);

        inline BusStopComponent* getCurrentBusStop() { return _currentBusStop; }
        inline float getDistanceToCurrentBusStop() { return _distanceToCurrentBusStop; }
        inline BusStopComponent* getBusStop(int index) { return _busStops[index]; }
        inline int getBusStopsCount() { return _busStops.size(); }

        void update(float deltaTime, Bus* bus);

};


#endif // TRANSITSYSTEM_H_INCLUDED
