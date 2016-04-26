#include "PhysicalBody.hpp"

#include "../Utils/Helpers.hpp"

// =========================================
// CONSTRUCTOR & DESTRUCTOR

PhysicalBody::PhysicalBody(btScalar m, btVector3 pos)
: _rigidBody(0), _collShape(0), _motionState(0),
_mass(m), _position(pos)
{

}

PhysicalBody::~PhysicalBody()
{
    printf("Physical body - Destruktor\n");
    safe_delete<btRigidBody>(_rigidBody);
    safe_delete<btCollisionShape>(_collShape);
    safe_delete<btDefaultMotionState>(_motionState);
}

// =========================================

void PhysicalBody::getTransform(btTransform& t)
{
    _motionState->getWorldTransform(t);
}
