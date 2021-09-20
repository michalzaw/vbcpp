#include "PhysicalBodySphere.hpp"

#include "../Utils/Helpers.hpp"
#include "../Utils/Logger2.h"

PhysicalBodySphere::PhysicalBodySphere(btScalar r, btScalar mass)
: PhysicalBody(mass),
_radius(r)
{
    updateBody();
}

PhysicalBodySphere::~PhysicalBodySphere()
{
    LOG_INFO("PhysicalBodySphere - Destruktor");
}


// Virtual function implementation

void PhysicalBodySphere::updateBody()
{
    _collShape.reset( new btSphereShape(_radius) );

    btVector3 inertia(0, 0, 0);
    _collShape->calculateLocalInertia(_mass, inertia);

    /* btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 70, 0)));

   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape->getShape(), fallInertia);
    _rigidBody = new btRigidBody(fallRigidBodyCI);

    */

    _motionState.reset( new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), _position)) );

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, _motionState.get(), _collShape.get(), inertia);
    _rigidBody.reset( new btRigidBody(rigidBodyCI) );
}
