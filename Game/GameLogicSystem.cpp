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


PathComponent* GameLogicSystem::addPathComponent(PathDirection direction)
{
	PathComponent* component = new PathComponent(direction);

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


void GameLogicSystem::setPathConnection(PathComponent* path1, const std::string& path2Name, int indexInPath1, int indexInPath2, bool registerAsPendingIfNotFound/* = true*/)
{
	auto path2 = std::find_if(_pathComponents.begin(), _pathComponents.end(), [&path2Name](PathComponent* path)
		{
			return path->getSceneObject() != nullptr && path->getSceneObject()->getName() == path2Name;
		});

	if (path2 != _pathComponents.end())
	{
		LOG_DEBUG("Path to connection: " + Strings::toString(path2Name) + " found.");
		path1->setConnection(indexInPath1, *path2, indexInPath2);
	}
	else if (registerAsPendingIfNotFound)
	{
		LOG_DEBUG("Path to connection: " + Strings::toString(path2Name) + " not found. Path is registerd as pending to connection.");
		_pendingPathConnections.push_back({ path1, path2Name, indexInPath1, indexInPath2 });
	}
	else
	{
		LOG_ERROR("Path to connection: " + Strings::toString(path2Name) + " not found.");
	}
}


void GameLogicSystem::createPendingPathConnections()
{
	for (const auto& pendingPathConnection : _pendingPathConnections)
	{
		setPathConnection(pendingPathConnection.path1, pendingPathConnection.path2Name, pendingPathConnection.indexInPath1, pendingPathConnection.indexInPath2, false);
	}

	_pendingPathConnections.clear();
}


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
