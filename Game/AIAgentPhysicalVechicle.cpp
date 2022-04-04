#include "AIAgentPhysicalVechicle.h"

#include "PathComponent.h"

#include "../Physics/PhysicalBodyRaycastVehicle.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Utils/QuaternionUtils.h"


AIAgentPhysicalVechicle::AIAgentPhysicalVechicle(PhysicalBodyRaycastVehicle* vechicle)
	: _vechicle(vechicle)
{

}


void AIAgentPhysicalVechicle::update(float deltaTime)
{
	//LOG_DEBUG("UPDATE 1");
	glm::vec3& currentPoint = _currentPath->getCurvePoints()[_currentPointIndex];
	if (getSceneObject()->getSceneManager()->getPhysicsManager()->isPointInObject(glm::vec3(currentPoint.x, currentPoint.y + 2.0f, currentPoint.z), _vechicle))
	{
		++_currentPointIndex;
		//LOG_DEBUG(toString(_currentPointIndex) + "/" + toString(_currentPath->getCurvePoints().size()));
	}

	currentPoint = _currentPath->getCurvePoints()[_currentPointIndex];
	glm::vec3& vechiclePosition = _vechicle->getSceneObject()->getPosition();

	btVector3& vechicleDirectionBt = _vechicle->getRayCastVehicle()->getForwardVector();
	glm::vec3& vechicleDirection = glm::normalize(glm::vec3(vechicleDirectionBt.x(), vechicleDirectionBt.y(), vechicleDirectionBt.z()));

	glm::vec3 destinationDirection = glm::normalize(currentPoint - vechiclePosition);

	float angle1 = atan2(vechicleDirection.z, vechicleDirection.x);
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

	angle *= 2.0f;

	if (angle < -0.55f)
	{
		angle = -0.55f;
	}
	if (angle > 0.55f)
	{
		angle = 0.55f;
	}

	//LOG_DEBUG(LOG_VARIABLE(angle2));
	//LOG_DEBUG(LOG_VARIABLE(destinationDirection));

	_vechicle->getRayCastVehicle()->setSteeringValue(angle, 0);
	_vechicle->getRayCastVehicle()->setSteeringValue(angle, 1);
	_vechicle->getRayCastVehicle()->setSteeringValue(angle * -0.5, 4);
	_vechicle->getRayCastVehicle()->setSteeringValue(angle * -0.5, 5);

	if (_vechicle->getRayCastVehicle()->getCurrentSpeedKmHour() < 40.0f)
	{
		_vechicle->getRayCastVehicle()->setBrake(0.0f, 2);
		_vechicle->getRayCastVehicle()->setBrake(0.0f, 3);

		_vechicle->getRayCastVehicle()->applyEngineForce(300.0f, 2);
		_vechicle->getRayCastVehicle()->applyEngineForce(300.0f, 3);
	}
	else
	{
		_vechicle->getRayCastVehicle()->setBrake(1.0f, 2);
		_vechicle->getRayCastVehicle()->setBrake(1.0f, 3);

		_vechicle->getRayCastVehicle()->applyEngineForce(0.0f, 2);
		_vechicle->getRayCastVehicle()->applyEngineForce(0.0f, 3);
	}

	LOG_DEBUG(toString(_vechicle->getRayCastVehicle()->getCurrentSpeedKmHour()));
}
