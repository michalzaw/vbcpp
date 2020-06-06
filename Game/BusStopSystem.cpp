#include "BusStopSystem.h"

static std::unique_ptr<BusStopSystem> instance;


BusStopSystem& BusStopSystem::getInstance()
{
    if ( !instance )
        instance = std::unique_ptr<BusStopSystem>(new BusStopSystem);

    return *instance;
}
