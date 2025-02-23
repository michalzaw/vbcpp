#include "PhysicalBodyRaycastVehicle.h"

#include "PhysicsManager.hpp"


PhysicalBodyRaycastVehicle::PhysicalBodyRaycastVehicle(Vertex* vertices, unsigned int vertexCount, btScalar mass, PhysicsManager* physicsManager)
    : PhysicalBodyConvexHull(vertices, vertexCount, mass),
    _physMgr(physicsManager),
    _maxSteeringAngle(DEFAULT_MAX_STEERING_ANGLE), _steeringValue(0.0f), _engineForce(0.0f), _brakeValue(1.0f)
{
    _physicalBodyType = PhysicalBodyType::RAYCAST_VEHICLE;

    _rayCaster = new RayCasterWithCollisionGroups(_physMgr->getDynamicsWorld());

    updateBody();
}


PhysicalBodyRaycastVehicle::PhysicalBodyRaycastVehicle(glm::vec3* vertices, unsigned int vertexCount, btScalar mass, PhysicsManager* physicsManager)
    : PhysicalBodyConvexHull(vertices, vertexCount, mass),
    _physMgr(physicsManager),
    _maxSteeringAngle(DEFAULT_MAX_STEERING_ANGLE), _steeringValue(0.0f), _engineForce(0.0f), _brakeValue(1.0f)
{
    _physicalBodyType = PhysicalBodyType::RAYCAST_VEHICLE;

    _rayCaster = new RayCasterWithCollisionGroups(_physMgr->getDynamicsWorld());

    updateBody();
}


PhysicalBodyRaycastVehicle::PhysicalBodyRaycastVehicle(std::vector<glm::vec3>& vertices, btScalar mass, PhysicsManager* physicsManager)
    : PhysicalBodyConvexHull(vertices, mass),
    _physMgr(physicsManager),
    _maxSteeringAngle(DEFAULT_MAX_STEERING_ANGLE), _steeringValue(0.0f), _engineForce(0.0f), _brakeValue(1.0f)
{
    _physicalBodyType = PhysicalBodyType::RAYCAST_VEHICLE;

    _rayCaster = new RayCasterWithCollisionGroups(_physMgr->getDynamicsWorld());

    updateBody();
}


PhysicalBodyRaycastVehicle::~PhysicalBodyRaycastVehicle()
{
    _physMgr->getDynamicsWorld()->removeVehicle(_rayCastVehicle);

    delete _rayCastVehicle;
    delete _rayCaster;
}


void PhysicalBodyRaycastVehicle::setWheelCollisionFilter(short int collisionGroup, short int collisionFilter)
{
    _rayCaster->setCollisionFilter(collisionGroup, collisionFilter);
}


int PhysicalBodyRaycastVehicle::addWheel(btVector3 connectionPoint, float suspensionRestLength, float radius, bool isFrontWheel)
{
    int index = _rayCastVehicle->getNumWheels();

    _rayCastVehicle->addWheel(connectionPoint, WHEEL_DIRECTION, WHEEL_AXLE, suspensionRestLength,
                              radius, _vehicleTuning, isFrontWheel);

    return index;
}


btRaycastVehicle* PhysicalBodyRaycastVehicle::getRayCastVehicle()
{
    return _rayCastVehicle;
}


void PhysicalBodyRaycastVehicle::setMaxSteeringAngle(float maxSteeringAngle)
{
    _maxSteeringAngle = maxSteeringAngle;
}


float PhysicalBodyRaycastVehicle::getMaxSteeringAngle()
{
    return _maxSteeringAngle;
}


void PhysicalBodyRaycastVehicle::setSteeringValue(float steering)
{
    _steeringValue = clamp(steering, -1.0f, 1.0f);

    for (int i = 0; i < _rayCastVehicle->getNumWheels(); ++i)
    {
        if (_rayCastVehicle->getWheelInfo(i).m_bIsFrontWheel)
        {
            _rayCastVehicle->setSteeringValue(_steeringValue * _maxSteeringAngle, i);
        }
    }
}


float PhysicalBodyRaycastVehicle::getSteeringValue()
{
    return _steeringValue;
}


void PhysicalBodyRaycastVehicle::setEngineForce(float engineForce)
{
    _engineForce = engineForce;

    for (int i = 0; i < _rayCastVehicle->getNumWheels(); ++i)
    {
        if (!_rayCastVehicle->getWheelInfo(i).m_bIsFrontWheel)
        {
            _rayCastVehicle->applyEngineForce(_engineForce, i);
        }
    }
}


float PhysicalBodyRaycastVehicle::getEngineForce()
{
    return _engineForce;
}


void PhysicalBodyRaycastVehicle::setBrakeValue(float brakeValue)
{
    _brakeValue = std::max(brakeValue, 0.0f);

    for (int i = 0; i < _rayCastVehicle->getNumWheels(); ++i)
    {
        _rayCastVehicle->setBrake(_brakeValue, i);
    }
}


float PhysicalBodyRaycastVehicle::getBrakeValue()
{
    return _brakeValue;
}


void PhysicalBodyRaycastVehicle::updateBody()
{
    _rayCastVehicle = new btRaycastVehicle(_vehicleTuning, getRigidBody(), _rayCaster);
    _rayCastVehicle->setCoordinateSystem(0, 1, 2);
    _physMgr->getDynamicsWorld()->addVehicle(_rayCastVehicle);
}
