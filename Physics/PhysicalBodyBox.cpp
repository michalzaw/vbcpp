#include "PhysicalBodyBox.hpp"

#include "../Utils/Helpers.hpp"
#include "../Utils/Logger2.h"

PhysicalBodyBox::PhysicalBodyBox(btVector3 halfExtents, btScalar mass)
: PhysicalBody(mass),
_halfExtents(halfExtents)
{
    updateBody();
}

PhysicalBodyBox::~PhysicalBodyBox()
{
    LOG_INFO("PhysicalBodyBox - Destruktor");
}

void PhysicalBodyBox::updateBody()
{
   _collShape.reset( new btBoxShape(_halfExtents) );

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
