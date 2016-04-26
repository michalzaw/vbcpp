#include "PhysicalBodyBox.hpp"

#include "../Utils/Helpers.hpp"

PhysicalBodyBox::PhysicalBodyBox(btVector3 halfExtents, btScalar mass, btVector3 pos)
: PhysicalBody(mass, pos),
_halfExtents(halfExtents)
{
    updateBody();
}

PhysicalBodyBox::~PhysicalBodyBox()
{
    printf("PhysicalBodyBox - Destruktor\n");
}

void PhysicalBodyBox::updateBody()
{
    safe_delete<btRigidBody>(_rigidBody);
    safe_delete<btCollisionShape>(_collShape);
    safe_delete<btDefaultMotionState>(_motionState);


   _collShape = new btBoxShape(_halfExtents);

    btVector3 inertia(0, 0, 0);
    _collShape->calculateLocalInertia(_mass, inertia);

    /* btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 70, 0)));

   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape->getShape(), fallInertia);
    _rigidBody = new btRigidBody(fallRigidBodyCI);

    */

    _motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), _position));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, _motionState, _collShape, inertia);
    _rigidBody = new btRigidBody(rigidBodyCI);
}
