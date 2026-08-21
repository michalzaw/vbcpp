#ifndef TRANSITSYSTEM_H_INCLUDED
#define TRANSITSYSTEM_H_INCLUDED


#include <vector>
#include <cstdlib>
#include <memory>

#include "CurrentRouteData.h"
#include "BusStopComponent.h"

#include "../../Bus/Bus.h"


struct BusRoutes;
struct BusRoute;
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

        BusRoute* _currentBusRoute;

    public:
        TransitSystem();
        ~TransitSystem();

        BusStopComponent* addBusStopComponent(const std::string& name);

        void removeBusStop(BusStopComponent* busStop);

        LUAF inline BusStopComponent* getCurrentBusStop() { return _currentRouteData.getCurrentBusStop(); }
        LUAF inline float getDistanceToCurrentBusStop() { return _distanceToCurrentBusStop; }
        LUAF inline BusStopComponent* getBusStop(int index) { return _busStops[index]; }
        LUAF inline int getBusStopsCount() { return _busStops.size(); }
        LUAF BusStopComponent* findBusStopById(int id);

        void setRoutes(BusRoutes* routes);
        void setSchedule(Schedule* schedule);

        LUAF inline BusRoutes* getRoutes() { return _routes; }
        LUAF inline Schedule* getSchedule() { return _schedule; }

        LUAF inline const CurrentRouteData& getCurrentRouteData() { return _currentRouteData; }

        LUAF void setCurrentRoute(int lineIndex, int brigadeIndex, int routeIndex);

        LUAF void setCurrentBusRoute(BusRoute* busRoute);
        LUAF BusRoute* getCurrentBusRoute();

        void update(float deltaTime, Bus* bus);

};


#endif // TRANSITSYSTEM_H_INCLUDED
