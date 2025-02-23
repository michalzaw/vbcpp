#ifndef CAMERACONTROLSYSTEM_H_INCLUDED
#define CAMERACONTROLSYSTEM_H_INCLUDED


#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "AI/PathComponent.h"
#include "AI/TrafficLightsComponent.h"


class CameraControlComponent;
class BusStopComponent;
class CameraFPS;
class Bus;
class AIAgent;
class AIAgentPhysicalVechicle;
class AIAgentVehicle;
class StopComponent;
class TrafficLightsComponent;
class PhysicalBodyRaycastVehicle;
class BusStartPoint;


struct PendingPathConnection final
{
	PathComponent* path1;
	std::string path2Name;
	int indexInPath1;
	int indexInPath2;
};


class GameLogicSystem final
{
	private:
		//std::vector<Bus*> _buses;

		std::vector<CameraControlComponent*> _cameraControlComponents;
		std::vector<PathComponent*> _pathComponents;
		std::vector<AIAgent*> _aiAgents;
		std::vector<AIAgentVehicle*> _aiAgentVehicles;
		std::vector<StopComponent*> _stopComponents;
		std::vector<TrafficLightsComponent*> _trafficLightsComponents;
		std::vector<BusStartPoint*> _busStartPoints; // todo: dodawac defaultowy Start point po wczytaniu sceny jesli vector jest pusty
		// todo: Przeniesc obsluge przystankow do tego systemu
		//std::vector<BusStopComponent*> _busStops;

		std::vector<PendingPathConnection> _pendingPathConnections;

	public:
		GameLogicSystem();
		~GameLogicSystem();

		//Bus* addBus(Bus* bus);
		CameraControlComponent* addCameraControlComponent(CameraFPS* camera);
		PathComponent* addPathComponent(PathDirection direction);
		AIAgent* addAIAgent();
		AIAgentPhysicalVechicle* addAIAgentPhysicalVechicle();
		AIAgentVehicle* addAIAgentVehicle();
		StopComponent* addStopComponent();
		TrafficLightsComponent* addTrafficLightsComponent(const std::string& redLightNodeName, const std::string& yellowLightNodeName, const std::string& greenLighNodeName,
														  const glm::vec3& triggerBoxPosition, const glm::vec3& triggerBoxRotation, const glm::vec3& triggerBoxSize,
														  const glm::vec3& stopPointPosition, const TrafficLightsState initState);
		BusStartPoint* addBusStartPoint(const std::string& name);
		//BusStopComponent* addBusStopComponent(std::string name);

		//void removeBus(Bus* bus);
		void removeCameraControlComponent(CameraControlComponent* component);
		void removePathComponent(PathComponent* component);
		void removeAIAgent(AIAgent* component);
		void removeAIAgentVehicle(AIAgentVehicle* component);
		void removeStopComponent(StopComponent* component);
		void removeTrafficLightsComponent(TrafficLightsComponent* component);
		void removeBusStartPoint(BusStartPoint* component);
		//void removeBusStop(BusStopComponent* busStop);

		void setPathConnection(PathComponent* path1, const std::string& path2Name, int indexInPath1, int indexInPath2, bool registerAsPendingIfNotFound = true);
		void createPendingPathConnections();

		//Bus* getBus(unsigned int index);
		inline const std::vector<PathComponent*>& getPathComponents() { return _pathComponents; }
		inline const std::vector<AIAgentVehicle*>& getAIAgentVehicles() { return _aiAgentVehicles; }
		inline const std::vector<StopComponent*>& getStopComponents() { return _stopComponents; }
		inline const std::vector<BusStartPoint*>& getBusStartPoints() { return _busStartPoints; }

		void update(float deltaTime);

		void destroy();
};


#endif // CAMERACONTROLSYSTEM_H_INCLUDED
