#include "PhysicalBodyRaycastVehicle.h"

#include "PhysicsManager.hpp"


PhysicalBodyRaycastVehicle::PhysicalBodyRaycastVehicle(Vertex* vertices, unsigned int vertexCount, btScalar mass, PhysicsManager* physicsManager)
    : PhysicalBodyConvexHull(vertices, vertexCount, mass),
    _physMgr(physicsManager)
{
    _rayCaster = new RayCasterWithCollisionGroups(_physMgr->getDynamicsWorld());

    updateBody();
}


PhysicalBodyRaycastVehicle::PhysicalBodyRaycastVehicle(glm::vec3* vertices, unsigned int vertexCount, btScalar mass, PhysicsManager* physicsManager)
    : PhysicalBodyConvexHull(vertices, vertexCount, mass),
    _physMgr(physicsManager)
{
    _rayCaster = new RayCasterWithCollisionGroups(_physMgr->getDynamicsWorld());

    updateBody();
}


PhysicalBodyRaycastVehicle::PhysicalBodyRaycastVehicle(std::vector<glm::vec3>& vertices, btScalar mass, PhysicsManager* physicsManager)
    : PhysicalBodyConvexHull(vertices, mass),
    _physMgr(physicsManager)
{
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


void PhysicalBodyRaycastVehicle::updateBody()
{
    _rayCastVehicle = new btRaycastVehicle(_vehicleTuning, getRigidBody(), _rayCaster);
    _rayCastVehicle->setCoordinateSystem(0, 1, 2);
    _physMgr->getDynamicsWorld()->addVehicle(_rayCastVehicle);
}
