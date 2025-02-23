#include "AIAgentVehicle.h"

#include "PathComponent.h"
#include "TrafficLightsComponent.h"

#include "../../Graphics/BezierCurve.h"

#include "../../Physics/PhysicalBodyRaycastVehicle.h"

#include "../../Scene/SceneManager.h"


AIAgentVehicle::AIAgentVehicle()
	: Component(CT_AI_AGENT_VEHICLE),
	_vehicle(nullptr), _currentPath(nullptr), _currentPathBezierCurve(nullptr),
	_isStop(false), _timeToStart(0.0f)
{

}


void AIAgentVehicle::onAttachedToScenObject()
{
	_vehicle = getSceneObject()->getComponentWithCasting<PhysicalBodyRaycastVehicle>(CT_PHYSICAL_BODY);
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


float AIAgentVehicle::getSteeringValue()
{
	const glm::vec3& vehiclePosition = _vehicle->getSceneObject()->getPosition();

	glm::vec3 closestTangent = _currentPathBezierCurve->getClosestTangentToPosition(vehiclePosition);

	int nextPointIndex = _currentPathBezierCurve->getClosestPointToPosition(vehiclePosition + closestTangent * 5.0f);
	const glm::vec3& nextPointPosition = _currentPathBezierCurve->getCurvePoints()[nextPointIndex];

	float destinationRotation = calculateDestinationRotation(nextPointPosition);

	return mapRange(-0.25f * PI, 0.25f * PI, -1.0f, 1.0f, destinationRotation);
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


void AIAgentVehicle::lookForward()
{
	PhysicsManager* physicsManager = _vehicle->getSceneObject()->getSceneManager()->getPhysicsManager();

	btVector3 forwardVector = _vehicle->getRayCastVehicle()->getForwardVector();

	glm::vec3 rayOrigin = getSceneObject()->transformLocalPointToGlobal(_frontSensorPosition);
	glm::vec3 rayDirection = glm::normalize(glm::vec3(forwardVector.x(), forwardVector.y(), forwardVector.z()));

	float rayLength = std::max(_vehicle->getRayCastVehicle()->getCurrentSpeedKmHour() / 2.0f, 1.1f);// 30.0f;

	short RAY_TEST_FILTER_MASK = btBroadphaseProxy::AllFilter;// COL_ENV | COL_BUS;
	short RAY_TEST_FILTER_GROUP = btBroadphaseProxy::DefaultFilter;// COL_WHEEL;

	glm::vec3 outPosition;
	PhysicalBody* outObject = nullptr;
	bool result = physicsManager->rayTest(rayOrigin, rayDirection, RAY_TEST_FILTER_MASK, RAY_TEST_FILTER_GROUP, outPosition, outObject, rayLength);
	if (result)
	{
		float distanceToObject = glm::distance(outPosition, rayOrigin);

		if (distanceToObject > 1.0f)
		{
			distanceToObject = distanceToObject - 1.0f;
		}
		else
		{
			distanceToObject = 0.0f;
		}
		stop(distanceToObject);
	}
}


void AIAgentVehicle::stop(float distance)
{
	_isStop = true;
	_timeToStart = 1.0f;
	
	if (distance > 0.0f)
	{
		float v0 = _vehicle->getRayCastVehicle()->getCurrentSpeedKmHour() * 1000.0f / 3600.0f; // m/s
		float a = (v0 * v0) / (2.0f * distance);
		float mass = _vehicle->getMass();
		_brakeForce = mass * a;
	}
	else
	{
		_brakeForce = 100.0f;
	}
}


void AIAgentVehicle::stopOnPoint(const glm::vec3& position)
{
	float distance = glm::distance(position, getSceneObject()->getPosition());

	distance -= _frontSensorPosition.z;

	stop(distance);
}


void AIAgentVehicle::update(float deltaTime)
{
	if (_currentPath == nullptr || _vehicle == nullptr)
	{
		return;
	}
	
	float steeringValue = getSteeringValue();

	_vehicle->setSteeringValue(steeringValue);

	lookForward();

	if (!_isStop)
	{
		if (_vehicle->getRayCastVehicle()->getCurrentSpeedKmHour() < 40.0f)
		{
			_vehicle->setBrakeValue(0.0f);
			_vehicle->setEngineForce(300.0f);
		}
		else
		{
			_vehicle->setBrakeValue(1.0f);
			_vehicle->setEngineForce(0.0f);
		}
	}
	else
	{
		_vehicle->setBrakeValue(_brakeForce * deltaTime / _vehicle->getRayCastVehicle()->getNumWheels());
		_vehicle->setEngineForce(0.0f);

		_timeToStart -= deltaTime;
		if (_timeToStart <= 0.0f)
		{
			_isStop = false;
		}
	}
}
