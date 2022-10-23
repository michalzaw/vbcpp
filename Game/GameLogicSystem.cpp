#include "GameLogicSystem.h"

#include <memory>

#include "AIAgent.h"
#include "AIAgentPhysicalVechicle.h"
#include "BusStopComponent.h"
#include "CameraControlComponent.h"
#include "PathComponent.h"

#include "../Graphics/CameraFPS.hpp"


GameLogicSystem::GameLogicSystem()
{

}


GameLogicSystem::~GameLogicSystem()
{
	destroy();
}


/*Bus* GameLogicSystem::addBus(Bus* bus)
{
	_buses.push_back(bus);

	return bus;
}*/


CameraControlComponent* GameLogicSystem::addCameraControlComponent(CameraFPS* camera)
{
	CameraControlComponent* component = new CameraControlComponent(camera);

	_cameraControlComponents.push_back(component);

	return component;

}


PathComponent* GameLogicSystem::addPathComponent(const std::vector<glm::vec3>& baseBezierCurveControlPoints, const glm::vec2& distanceFromBaseBezierCurve,
												 float marginBegin/* = 0.0f*/, float marginEnd/* = 0.0f*/)
{
	PathComponent* component = new PathComponent(baseBezierCurveControlPoints, distanceFromBaseBezierCurve, marginBegin, marginEnd);

	_pathComponents.push_back(component);

	return component;
}


AIAgent* GameLogicSystem::addAIAgent()
{
	AIAgent* component = new AIAgent;

	_aiAgents.push_back(component);

	return component;
}


AIAgent* GameLogicSystem::addAIAgent(PhysicalBodyRaycastVehicle* vechicle)
{
	AIAgent* component = new AIAgentPhysicalVechicle(vechicle);

	_aiAgents.push_back(component);

	return component;
}


/*BusStopComponent* GameLogicSystem::addBusStopComponent(std::string name)
{
	BusStopComponent* busStop = new BusStopComponent(name);

	_busStops.push_back(busStop);

	return busStop;
}*/


/*void GameLogicSystem::removeBus(Bus* bus)
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
}*/


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


void GameLogicSystem::removePathComponent(PathComponent* component)
{
	for (std::vector<PathComponent*>::iterator i = _pathComponents.begin(); i != _pathComponents.end(); ++i)
	{
		if (*i == component)
		{
			i = _pathComponents.erase(i);

			delete component;

			return;
		}
	}
}


void GameLogicSystem::removeAIAgent(AIAgent* component)
{
	for (std::vector<AIAgent*>::iterator i = _aiAgents.begin(); i != _aiAgents.end(); ++i)
	{
		if (*i == component)
		{
			i = _aiAgents.erase(i);

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


/*Bus* GameLogicSystem::getBus(unsigned int index)
{
	return _buses[index];
}*/


void GameLogicSystem::update(float deltaTime)
{
	for (CameraControlComponent* component : _cameraControlComponents)
	{
		component->update(deltaTime);
	}

	for (PathComponent* component : _pathComponents)
	{
		component->update(deltaTime);
	}

	for (AIAgent* component : _aiAgents)
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
	/*for (std::vector<Bus*>::iterator i = _buses.begin(); i != _buses.end(); ++i)
	{
		delete* i;
	}

	_buses.clear();*/

	for (std::vector<CameraControlComponent*>::iterator i = _cameraControlComponents.begin(); i != _cameraControlComponents.end(); ++i)
	{
		delete* i;
	}

	_cameraControlComponents.clear();

	for (std::vector<PathComponent*>::iterator i = _pathComponents.begin(); i != _pathComponents.end(); ++i)
	{
		delete* i;
	}

	_pathComponents.clear();

	for (std::vector<AIAgent*>::iterator i = _aiAgents.begin(); i != _aiAgents.end(); ++i)
	{
		delete* i;
	}

	_aiAgents.clear();

	/*for (std::vector<BusStopComponent*>::iterator i = _busStops.begin(); i != _busStops.end(); ++i)
	{
		delete* i;
	}

	_busStops.clear();*/
}
