#ifndef CAMERACONTROLSYSTEM_H_INCLUDED
#define CAMERACONTROLSYSTEM_H_INCLUDED


#include <vector>
#include <string>

#include <glm/glm.hpp>

class CameraControlComponent;
class BusStopComponent;
class CameraFPS;
class Bus;
class PathComponent;
class AIAgent;
class PhysicalBodyRaycastVehicle;
enum PathDirection;


class GameLogicSystem final
{
	private:
		//std::vector<Bus*> _buses;

		std::vector<CameraControlComponent*> _cameraControlComponents;
		std::vector<PathComponent*> _pathComponents;
		std::vector<AIAgent*> _aiAgents;
		// todo: Przeniesc obsluge przystankow do tego systemu
		//std::vector<BusStopComponent*> _busStops;

	public:
		GameLogicSystem();
		~GameLogicSystem();

		//Bus* addBus(Bus* bus);
		CameraControlComponent* addCameraControlComponent(CameraFPS* camera);
		PathComponent* addPathComponent(const std::vector<glm::vec3>& baseBezierCurveControlPoints, const glm::vec2& distanceFromBaseBezierCurve, PathDirection direction,
										float marginBegin = 0.0f, float marginEnd = 0.0f);
		AIAgent* addAIAgent();
		AIAgent* addAIAgent(PhysicalBodyRaycastVehicle* vechicle);
		//BusStopComponent* addBusStopComponent(std::string name);

		//void removeBus(Bus* bus);
		void removeCameraControlComponent(CameraControlComponent* component);
		void removePathComponent(PathComponent* component);
		void removeAIAgent(AIAgent* component);
		//void removeBusStop(BusStopComponent* busStop);

		//Bus* getBus(unsigned int index);

		void update(float deltaTime);

		void destroy();
};


#endif // CAMERACONTROLSYSTEM_H_INCLUDED
