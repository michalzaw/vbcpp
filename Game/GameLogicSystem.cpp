#include "GameLogicSystem.h"

#include <memory>

#include "BusStopComponent.h"
#include "CameraControlComponent.h"

#include "../Graphics/CameraFPS.hpp"


static std::unique_ptr<GameLogicSystem> instance;


GameLogicSystem::GameLogicSystem()
{

}


GameLogicSystem::~GameLogicSystem()
{
	destroy();
}


GameLogicSystem& GameLogicSystem::getInstance()
{
	if (!instance)
		instance = std::unique_ptr<GameLogicSystem>(new GameLogicSystem);

	return *instance;
}


Bus* GameLogicSystem::addBus(Bus* bus)
{
	_buses.push_back(bus);

	return bus;
}


CameraControlComponent* GameLogicSystem::addCameraControlComponent(CameraFPS* camera)
{
	CameraControlComponent* component = new CameraControlComponent(camera);

	_cameraControlComponents.push_back(component);

	return component;

}


/*BusStopComponent* GameLogicSystem::addBusStopComponent(std::string name)
{
	BusStopComponent* busStop = new BusStopComponent(name);

	_busStops.push_back(busStop);

	return busStop;
}*/


void GameLogicSystem::removeBus(Bus* bus)
{
	for (std::vector<Bus*>::iterator i = _buses.begin(); i != _buses.end(); ++i)
	{
		if (*i == bus)
		{
			i = _buses.erase(i);

			bus->drop();

			return;
		}
	}
}


void GameLogicSystem::removeCameraControlComponent(CameraControlComponent* component)
{
	for (std::vector<CameraControlComponent*>::iterator i = _cameraControlComponents.begin(); i != _cameraControlComponents.end(); ++i)
	{
		if (*i == component)
		{
			i = _cameraControlComponents.erase(i);

			delete component;

			return;
		}
	}
}


/*void GameLogicSystem::removeBusStop(BusStopComponent* busStop)
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
}*/


Bus* GameLogicSystem::getBus(unsigned int index)
{
	return _buses[index];
}


void GameLogicSystem::update(float deltaTime)
{
	for (CameraControlComponent* component : _cameraControlComponents)
	{
		component->update(deltaTime);
	}

	/*for (BusStopComponent* component : _busStops)
	{
		component->update(deltaTime);
	}*/
}


void GameLogicSystem::destroy()
{
	for (std::vector<Bus*>::iterator i = _buses.begin(); i != _buses.end(); ++i)
	{
		delete* i;
	}

	_buses.clear();

	for (std::vector<CameraControlComponent*>::iterator i = _cameraControlComponents.begin(); i != _cameraControlComponents.end(); ++i)
	{
		delete* i;
	}

	_cameraControlComponents.clear();

	/*for (std::vector<BusStopComponent*>::iterator i = _busStops.begin(); i != _busStops.end(); ++i)
	{
		delete* i;
	}

	_busStops.clear();*/
}
