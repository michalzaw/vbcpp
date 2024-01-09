#include "AIAgentVehicle.h"

#include "PathComponent.h"

#include "../Physics/PhysicalBodyRaycastVehicle.h"


AIAgentVehicle::AIAgentVehicle()
	: Component(CT_AI_AGENT_VEHICLE)
{

}


void AIAgentVehicle::onAttachedToScenObject()
{
	_vehicle = dynamic_cast<PhysicalBodyRaycastVehicle*>(getSceneObject()->getComponent(CT_PHYSICAL_BODY));
	if (_vehicle != nullptr)
	{

	}
}


void AIAgentVehicle::setCurrentPath(PathComponent* path)
{
	if (path != nullptr)
	{
		path->getSceneObject()->addChild(getSceneObject());
	}
	else
	{
		getSceneObject()->getParent()->removeChild(getSceneObject());
	}

	_currentPath = path;
}


void AIAgentVehicle::update(float deltaTime)
{

}
