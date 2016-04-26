#include "PhysicalBodySphere.hpp"

#include "../Utils/Helpers.hpp"

PhysicalBodySphere::PhysicalBodySphere(btScalar r, btScalar mass, btVector3 pos)
: PhysicalBody(mass, pos),
_radius(r)
{
    updateBody();
}

PhysicalBodySphere::~PhysicalBodySphere()
{
    printf("PhysicalBodyBox - Destruktor\n");
}


// Virtual function implementation

void PhysicalBodySphere::updateBody()
{
    safe_delete<btRigidBody>(_rigidBody);
    safe_delete<btCollisionShape>(_collShape);
    safe_delete<btDefaultMotionState>(_motionState);

    _collShape = new btSphereShape(_radius);

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
