#include "PhysicalBody.hpp"

#include "../Utils/Helpers.hpp"

// =========================================
// CONSTRUCTOR & DESTRUCTOR

PhysicalBody::PhysicalBody(btScalar m, btVector3 pos, btVector3 rot)
: Component(CT_PHYSICAL_BODY),
_rigidBody(nullptr), _collShape(nullptr), _motionState(nullptr),
_mass(m), _position(pos), _rotation(rot)
{

}

PhysicalBody::~PhysicalBody()
{
    printf("Physical body - Destruktor\n");

}

// =========================================

/*void PhysicalBody::getTransform(btTransform& t)
{
    _motionState->getWorldTransform(t);
}*/


void PhysicalBody::setRotation(btVector3 rot)
{
    btTransform transf;
    transf.setIdentity();

    btQuaternion quat;
    quat.setEuler(rot.x(), rot.y(), rot.z());

    transf.setRotation(quat);
    _rigidBody->setCenterOfMassTransform(transf);
}


void PhysicalBody::update()
{
    if (_object->isActive())
    {
        if (_mass > 0)
        {
            btTransform transf;
            _motionState->getWorldTransform(transf);

            _object->setPosition(glm::vec3(transf.getOrigin().getX(),transf.getOrigin().getY(),transf.getOrigin().getZ()));
            _object->setRotationQuaternion(transf.getRotation().getX(),transf.getRotation().getY(),transf.getRotation().getZ(),transf.getRotation().getW());
        }
    }
}
