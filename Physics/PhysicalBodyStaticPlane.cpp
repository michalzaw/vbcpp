#include "PhysicalBodyStaticPlane.hpp"

#include "../Utils/Helpers.hpp"

PhysicalBodyStaticPlane::PhysicalBodyStaticPlane(btVector3 planeNormal, btScalar offset)
: PhysicalBody(0),
_offset(offset)
{
    updateBody();
}

PhysicalBodyStaticPlane::~PhysicalBodyStaticPlane()
{
    printf("PhysicalBodyBox - Destruktor\n");
}


void PhysicalBodyStaticPlane::updateBody()
{
    /*
        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 2);

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -2, 0)));
    btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    physicsMgr->getDynamicsWorld()->addRigidBody(groundRigidBody);
    */

    _collShape.reset( new btStaticPlaneShape(_position, _offset) );

    _motionState.reset( new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))) );

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, _motionState.get(), _collShape.get(), btVector3(0, 0, 0));
    _rigidBody.reset( new btRigidBody(rigidBodyCI) );
}
