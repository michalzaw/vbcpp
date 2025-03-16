#include "AIAgentVehicle.h"

#include "PathComponent.h"
#include "TrafficLightsComponent.h"

#include "../../Graphics/BezierCurve.h"

#include "../../Physics/PhysicalBodyRaycastVehicle.h"

#include "../../Scene/SceneManager.h"


AIAgentVehicle::AIAgentVehicle()
	: Component(CT_AI_AGENT_VEHICLE),
	_vehicle(nullptr), _currentPath(nullptr), _currentPathBezierCurve(nullptr),
	_nextPath(nullptr),
	_frontSensorPosition(0.0f, 0.0f, 0.0f),
	_isStop(false), _timeToStart(0.0f), _brakeForce(0.0f),
	_targetSpeed(0.0f),
	_nextPointIndex(0)
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
	_currentPath = path;

	if (path != nullptr)
	{
		_currentPath->getSceneObject()->addChild(getSceneObject());

		_currentPathBezierCurve = dynamic_cast<BezierCurve*>(_currentPath->getSceneObject()->getComponent(CT_BEZIER_CURVE));

		_targetSpeed = _currentPath->getMaxSpeed();
	}
	else
	{
		getSceneObject()->getParent()->removeChild(getSceneObject());
	}
}


float AIAgentVehicle::getSteeringValue()
{
	const glm::vec3& vehiclePosition = _vehicle->getSceneObject()->getPosition();

	glm::vec3 closestTangent = _currentPathBezierCurve->getClosestTangentToPosition(vehiclePosition);

	_nextPointIndex = _currentPathBezierCurve->getClosestPointToPosition(vehiclePosition + closestTangent * 5.0f);
	const glm::vec3& nextPointPosition = _currentPathBezierCurve->getCurvePoints()[_nextPointIndex];

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


bool AIAgentVehicle::isNearToPathEnd(float expectedDistance)
{
	float distanceToPathEnd = glm::distance(getSceneObject()->getPosition(), _currentPathBezierCurve->getCurvePoints()[_nextPointIndex]);

	for (int i = _nextPointIndex; i < _currentPathBezierCurve->getCurvePoints().size() - 1; ++i)
	{
		const glm::vec3& p1 = _currentPathBezierCurve->getCurvePoints()[i];
		const glm::vec3& p2 = _currentPathBezierCurve->getCurvePoints()[i + 1];

		distanceToPathEnd += glm::distance(p1, p2);

		if (distanceToPathEnd > expectedDistance)
		{
			return false;
		}
	}

	return true;
}


void AIAgentVehicle::chooseNextPath()
{
	const auto& nextPaths = _currentPath->getNextPaths();
	if (nextPaths.size() > 0)
	{
		//int nexPathIndex = nextPaths.size() > 1 ? 1 : 0;
		int nextPathIndex =  rand() % nextPaths.size();
		_nextPath = nextPaths[nextPathIndex].path;

		float nextPathMaxSpeed = _nextPath->getMaxSpeed();
		if (nextPathMaxSpeed < _targetSpeed)
		{
			_targetSpeed = _nextPath->getMaxSpeed();
		}

		LOG_DEBUG("Chosen next path=" + Strings::toString(nextPathIndex));
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


void AIAgentVehicle::brake(float brakeForce)
{
	_isStop = true;
	_timeToStart = 1.0f;

	_brakeForce = brakeForce;
}


void AIAgentVehicle::update(float deltaTime)
{
	if (_currentPath == nullptr || _vehicle == nullptr)
	{
		return;
	}
	
	float steeringValue = getSteeringValue();

	if (_nextPath == nullptr && isNearToPathEnd(20.0f))
	{
		chooseNextPath();
	}
	else if (_nextPath != nullptr && _nextPointIndex == _currentPathBezierCurve->getCurvePoints().size() - 1)
	{
		setCurrentPath(_nextPath);
		_nextPath = nullptr;
	}

	_vehicle->setSteeringValue(steeringValue);

	lookForward();

	if (!_isStop)
	{
		if (_vehicle->getRayCastVehicle()->getCurrentSpeedKmHour() - 5.0f > _targetSpeed)
		{
			brake(1200.0f);
		}

		if (_vehicle->getRayCastVehicle()->getCurrentSpeedKmHour() < _targetSpeed)
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

	//LOG_DEBUG(LOG_VARIABLE(_vehicle->getRayCastVehicle()->getCurrentSpeedKmHour()));
}
