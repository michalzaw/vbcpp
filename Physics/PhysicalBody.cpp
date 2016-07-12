#include "PhysicalBody.hpp"

#include "../Utils/Helpers.hpp"

// =========================================
// CONSTRUCTOR & DESTRUCTOR

PhysicalBody::PhysicalBody(btScalar m, btVector3 pos)
: Component(CT_PHYSICAL_BODY),
_rigidBody(0), _collShape(0), _motionState(0),
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

/*void PhysicalBody::getTransform(btTransform& t)
{
    _motionState->getWorldTransform(t);
}*/

void PhysicalBody::update()
{
    if (_object->isActive())
    {
        if (_mass > 0)
        {
            btTransform transf;
            _motionState->getWorldTransform(transf);

            _objectTransform->SetPosition(glm::vec3(transf.getOrigin().getX(),transf.getOrigin().getY(),transf.getOrigin().getZ()));
            _objectTransform->SetRotation(transf.getRotation().getX(),transf.getRotation().getY(),transf.getRotation().getZ(),transf.getRotation().getW());
        }
    }
}
