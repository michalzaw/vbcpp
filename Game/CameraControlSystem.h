#ifndef CAMERACONTROLSYSTEM_H_INCLUDED
#define CAMERACONTROLSYSTEM_H_INCLUDED


#include <vector>

#include "CameraControlComponent.h"


class CameraControlSystem
{
	private:
		std::vector<CameraControlComponent*> _cameraControlComponents;

		CameraControlSystem();

	public:
		~CameraControlSystem();

		static CameraControlSystem& getInstance();

		CameraControlComponent* addCameraControlComponent(CameraFPS* camera);

		void removeCameraControlComponent(CameraControlComponent* component);

		void setCameraActivity(CameraControlComponent* component, bool isActive);

		void update(float deltaTime);
};


#endif // CAMERACONTROLSYSTEM_H_INCLUDED
