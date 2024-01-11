#include "AIAgentVehicle.h"

#include "PathComponent.h"

#include "../Graphics/BezierCurve.h"

#include "../Physics/PhysicalBodyRaycastVehicle.h"


AIAgentVehicle::AIAgentVehicle()
	: Component(CT_AI_AGENT_VEHICLE),
	_vehicle(nullptr), _currentPath(nullptr), _currentPathBezierCurve(nullptr)
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
	_currentPathBezierCurve = dynamic_cast<BezierCurve*>(_currentPath->getSceneObject()->getComponent(CT_BEZIER_CURVE));
}


float mapRange(float a1, float a2, float b1, float b2, float value)
{
	return b1 + (value - a1) * (b2 - b1) / (a2 - a1);
}


float AIAgentVehicle::getSteeringValue()
{
	const glm::vec3& vehiclePosition = _vehicle->getSceneObject()->getPosition();

	glm::vec3 closestTangent = _currentPathBezierCurve->getClosestTangentToPosition(vehiclePosition);

	int nextPointIndex = _currentPathBezierCurve->getClosestPointToPosition(vehiclePosition + closestTangent * 5.0f);
	const glm::vec3& nextPointPosition = _currentPathBezierCurve->getCurvePoints()[nextPointIndex];

	float destinationRotation = calculateDestinationRotation(nextPointPosition);

	return mapRange(-0.25f * PI, 0.25f * PI, -0.55, 0.55, destinationRotation);
}


float AIAgentVehicle::calculateDestinationRotation(const glm::vec3& destinationPoint)
{
	const glm::vec3& vehiclePosition = _vehicle->getSceneObject()->getPosition();

	btVector3 vehicleDirectionBt = _vehicle->getRayCastVehicle()->getForwardVector();
	glm::vec3 vehicleDirection = glm::normalize(glm::vec3(vehicleDirectionBt.x(), vehicleDirectionBt.y(), vehicleDirectionBt.z()));

	glm::vec3 destinationDirection = glm::normalize(destinationPoint - vehiclePosition);

	float angle1 = atan2(vehicleDirection.z, vehicleDirection.x);
	float angle2 = atan2(destinationDirection.z, destinationDirection.x);

	float angle = angle1 - angle2;// acos(glm::dot(vechicleDirection, destinationDirection));

	// https://stackoverflow.com/questions/1878907/how-can-i-find-the-difference-between-two-angles
	if (angle > PI)
	{
		angle -= 2 * PI;
	}
	if (angle < -PI)
	{
		angle += 2 * PI;
	}

	return angle;
}


void AIAgentVehicle::update(float deltaTime)
{
	if (_currentPath == nullptr || _vehicle == nullptr)
	{
		return;
	}
	
	float steeringValue = getSteeringValue();

	_vehicle->getRayCastVehicle()->setSteeringValue(steeringValue, 0);
	_vehicle->getRayCastVehicle()->setSteeringValue(steeringValue, 1);


	if (_vehicle->getRayCastVehicle()->getCurrentSpeedKmHour() < 40.0f)
	{
		_vehicle->getRayCastVehicle()->setBrake(0.0f, 2);
		_vehicle->getRayCastVehicle()->setBrake(0.0f, 3);

		_vehicle->getRayCastVehicle()->applyEngineForce(300.0f, 2);
		_vehicle->getRayCastVehicle()->applyEngineForce(300.0f, 3);
	}
	else
	{
		_vehicle->getRayCastVehicle()->setBrake(1.0f, 2);
		_vehicle->getRayCastVehicle()->setBrake(1.0f, 3);

		_vehicle->getRayCastVehicle()->applyEngineForce(0.0f, 2);
		_vehicle->getRayCastVehicle()->applyEngineForce(0.0f, 3);
	}
}
