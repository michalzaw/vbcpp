#ifndef CAMERACONTROLSYSTEM_H_INCLUDED
#define CAMERACONTROLSYSTEM_H_INCLUDED


#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "PathComponent.h"


class CameraControlComponent;
class BusStopComponent;
class CameraFPS;
class Bus;
class AIAgent;
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
		AIAgent* addAIAgent(PhysicalBodyRaycastVehicle* vechicle);
		BusStartPoint* addBusStartPoint(const std::string& name);
		//BusStopComponent* addBusStopComponent(std::string name);

		//void removeBus(Bus* bus);
		void removeCameraControlComponent(CameraControlComponent* component);
		void removePathComponent(PathComponent* component);
		void removeAIAgent(AIAgent* component);
		void removeBusStartPoint(BusStartPoint* component);
		//void removeBusStop(BusStopComponent* busStop);

		void setPathConnection(PathComponent* path1, const std::string& path2Name, int indexInPath1, int indexInPath2, bool registerAsPendingIfNotFound = true);
		void createPendingPathConnections();

		//Bus* getBus(unsigned int index);
		inline const std::vector<PathComponent*>& getPathComponents() { return _pathComponents; }
		inline const std::vector<BusStartPoint*>& getBusStartPoints() { return _busStartPoints; }

		void update(float deltaTime);

		void destroy();
};


#endif // CAMERACONTROLSYSTEM_H_INCLUDED
