#include "GameLogicSystem.h"

#include <memory>

#include "AI/AIAgent.h"
#include "AI/AIAgentPhysicalVechicle.h"
#include "AI/AIAgentVehicle.h"
#include "AI/PathComponent.h"
#include "AI/StopComponent.h"
#include "AI/TrafficLightsComponent.h"
#include "BusStartPoint.h"
#include "BusStopComponent.h"
#include "CameraControlComponent.h"

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


AIAgentPhysicalVechicle* GameLogicSystem::addAIAgentPhysicalVechicle()
{
	AIAgentPhysicalVechicle* component = new AIAgentPhysicalVechicle();

	_aiAgents.push_back(component);

	return component;
}


AIAgentVehicle* GameLogicSystem::addAIAgentVehicle()
{
	AIAgentVehicle* component = new AIAgentVehicle();

	_aiAgentVehicles.push_back(component);

	return component;
}


StopComponent* GameLogicSystem::addStopComponent()
{
	StopComponent* component = new StopComponent();

	_stopComponents.push_back(component);

	return component;
}


TrafficLightsComponent* GameLogicSystem::addTrafficLightsComponent(const std::string& redLightNodeName, const std::string& yellowLightNodeName, const std::string& greenLighNodeName,
																   const glm::vec3& triggerBoxPosition, const glm::vec3& triggerBoxRotation, const glm::vec3& triggerBoxSize,
																   const glm::vec3& stopPointPosition, const TrafficLightsState initState)
{
	TrafficLightsComponent* component = new TrafficLightsComponent(redLightNodeName, yellowLightNodeName, greenLighNodeName, triggerBoxPosition, triggerBoxRotation, triggerBoxSize, stopPointPosition, initState);

	_trafficLightsComponents.push_back(component);

	return component;
}


BusStartPoint* GameLogicSystem::addBusStartPoint(const std::string& name)
{
	BusStartPoint* component = new BusStartPoint(name);

	_busStartPoints.push_back(component);

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


void GameLogicSystem::removeAIAgentVehicle(AIAgentVehicle* component)
{
	for (std::vector<AIAgentVehicle*>::iterator i = _aiAgentVehicles.begin(); i != _aiAgentVehicles.end(); ++i)
	{
		if (*i == component)
		{
			i = _aiAgentVehicles.erase(i);

			delete component;

			return;
		}
	}
}


void GameLogicSystem::removeStopComponent(StopComponent* component)
{
	for (std::vector<StopComponent*>::iterator i = _stopComponents.begin(); i != _stopComponents.end(); ++i)
	{
		if (*i == component)
		{
			i = _stopComponents.erase(i);

			delete component;

			return;
		}
	}
}


void GameLogicSystem::removeTrafficLightsComponent(TrafficLightsComponent* component)
{
	for (std::vector<TrafficLightsComponent*>::iterator i = _trafficLightsComponents.begin(); i != _trafficLightsComponents.end(); ++i)
	{
		if (*i == component)
		{
			i = _trafficLightsComponents.erase(i);

			delete component;

			return;
		}
	}
}


void GameLogicSystem::removeBusStartPoint(BusStartPoint* component)
{
	for (std::vector<BusStartPoint*>::iterator i = _busStartPoints.begin(); i != _busStartPoints.end(); ++i)
	{
		if (*i == component)
		{
			i = _busStartPoints.erase(i);

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
		if (component->isActive())
		{
			component->update(deltaTime);
		}
	}

	for (AIAgentVehicle* component : _aiAgentVehicles)
	{
		if (component->isActive())
		{
			component->update(deltaTime);
		}
	}

	for (StopComponent* component : _stopComponents)
	{
		if (component->isActive())
		{
			component->update(deltaTime);
		}
	}

	for (TrafficLightsComponent* component : _trafficLightsComponents)
	{
		if (component->isActive())
		{
			component->update(deltaTime);
		}
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

	for (std::vector<AIAgentVehicle*>::iterator i = _aiAgentVehicles.begin(); i != _aiAgentVehicles.end(); ++i)
	{
		delete* i;
	}

	_aiAgentVehicles.clear();

	for (std::vector<StopComponent*>::iterator i = _stopComponents.begin(); i != _stopComponents.end(); ++i)
	{
		delete* i;
	}

	_stopComponents.clear();

	for (std::vector<TrafficLightsComponent*>::iterator i = _trafficLightsComponents.begin(); i != _trafficLightsComponents.end(); ++i)
	{
		delete* i;
	}

	_trafficLightsComponents.clear();

	for (std::vector<BusStartPoint*>::iterator i = _busStartPoints.begin(); i != _busStartPoints.end(); ++i)
	{
		delete* i;
	}

	_busStartPoints.clear();

	/*for (std::vector<BusStopComponent*>::iterator i = _busStops.begin(); i != _busStops.end(); ++i)
	{
		delete* i;
	}

	_busStops.clear();*/
}
