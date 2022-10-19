#ifndef CAMERACONTROLSYSTEM_H_INCLUDED
#define CAMERACONTROLSYSTEM_H_INCLUDED


#include <vector>
#include <string>

class CameraControlComponent;
class BusStopComponent;
class CameraFPS;
class Bus;


class GameLogicSystem final
{
	private:
		//std::vector<Bus*> _buses;

		std::vector<CameraControlComponent*> _cameraControlComponents;
		// todo: Przeniesc obsluge przystankow do tego systemu
		//std::vector<BusStopComponent*> _busStops;

	public:
		GameLogicSystem();
		~GameLogicSystem();

		//Bus* addBus(Bus* bus);
		CameraControlComponent* addCameraControlComponent(CameraFPS* camera);
		//BusStopComponent* addBusStopComponent(std::string name);

		//void removeBus(Bus* bus);
		void removeCameraControlComponent(CameraControlComponent* component);
		//void removeBusStop(BusStopComponent* busStop);

		//Bus* getBus(unsigned int index);

		void update(float deltaTime);

		void destroy();
};


#endif // CAMERACONTROLSYSTEM_H_INCLUDED
