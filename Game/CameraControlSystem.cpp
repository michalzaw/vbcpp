#include "CameraControlSystem.h"

#include <memory>


static std::unique_ptr<CameraControlSystem> instance;


CameraControlSystem::CameraControlSystem()
{

}


CameraControlSystem::~CameraControlSystem()
{
	for (std::vector<CameraControlComponent*>::iterator i = _cameraControlComponents.begin(); i != _cameraControlComponents.end(); ++i)
	{
		delete* i;
	}
}


CameraControlSystem& CameraControlSystem::getInstance()
{
	if (!instance)
		instance = std::unique_ptr<CameraControlSystem>(new CameraControlSystem);

	return *instance;
}


CameraControlComponent* CameraControlSystem::addCameraControlComponent(CameraFPS* camera)
{
	CameraControlComponent* component = new CameraControlComponent(camera);

	_cameraControlComponents.push_back(component);

	return component;

}


void CameraControlSystem::removeCameraControlComponent(CameraControlComponent* component)
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


void CameraControlSystem::setCameraActivity(CameraControlComponent* component, bool isActive)
{
	for (CameraControlComponent* i : _cameraControlComponents)
	{
		if (component == i)
		{
			component->setIsActive(isActive);
		}
	}
}


void CameraControlSystem::update(float deltaTime)
{
	for (CameraControlComponent* component : _cameraControlComponents)
	{
		component->update(deltaTime);
	}
}
