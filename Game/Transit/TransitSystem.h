#ifndef TRANSITSYSTEM_H_INCLUDED
#define TRANSITSYSTEM_H_INCLUDED


#include <vector>
#include <cstdlib>
#include <memory>

#include "CurrentRouteData.h"
#include "BusStopComponent.h"

#include "../../Bus/Bus.h"


struct BusRoutes;
struct Schedule;


class TransitSystem
{
    private:
        static const int MAX_NUMBER_OF_PASSENGERS = 30;
        static constexpr float MIN_DISTANCE_TO_BUS_STOP = 10.0f;

        std::vector<BusStopComponent*> _busStops;

        float _distanceToCurrentBusStop;

        BusRoutes* _routes;
        Schedule* _schedule;

        CurrentRouteData _currentRouteData;

    public:
        TransitSystem();
        ~TransitSystem();

        BusStopComponent* addBusStopComponent(const std::string& name);

        void removeBusStop(BusStopComponent* busStop);

        inline BusStopComponent* getCurrentBusStop() { return _currentRouteData.isSet() ? findBusStopById(_currentRouteData.currentRoute->stops[_currentRouteData.currentBusStopIndex].id) : nullptr; }
        inline float getDistanceToCurrentBusStop() { return _distanceToCurrentBusStop; }
        inline BusStopComponent* getBusStop(int index) { return _busStops[index]; }
        inline int getBusStopsCount() { return _busStops.size(); }
        BusStopComponent* findBusStopById(int id);

        void setRoutes(BusRoutes* routes);
        void setSchedule(Schedule* schedule);

        inline BusRoutes* getRoutes() { return _routes; }
        inline Schedule* getSchedule() { return _schedule; }

        inline const CurrentRouteData& getCurrentRouteData() { return _currentRouteData; }

        void setCurrentRoute(int lineIndex, int brigadeIndex, int routeIndex);

        void update(float deltaTime, Bus* bus);

};


#endif // TRANSITSYSTEM_H_INCLUDED
