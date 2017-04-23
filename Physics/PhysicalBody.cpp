#include "PhysicalBody.hpp"

#include "../Utils/Helpers.hpp"

// =========================================
// CONSTRUCTOR & DESTRUCTOR

PhysicalBody::PhysicalBody(btScalar m, btVector3 pos, btVector3 rot)
: Component(CT_PHYSICAL_BODY),
_rigidBody(nullptr), _collShape(nullptr), _motionState(nullptr),
_mass(m), _position(pos), _rotation(rot),
_isUpdateTransformFromObject(true)
{

}

PhysicalBody::~PhysicalBody()
{
    printf("Physical body - Destruktor\n");

}


void PhysicalBody::update()
{
    if (_object->isActive())
    {
        if (_mass > 0)
        {
            btTransform transf;
            _motionState->getWorldTransform(transf);

            _isUpdateTransformFromObject = false;
            _object->setPosition(glm::vec3(transf.getOrigin().getX(),transf.getOrigin().getY(),transf.getOrigin().getZ()));
            _object->setRotationQuaternion(transf.getRotation().getX(),transf.getRotation().getY(),transf.getRotation().getZ(),transf.getRotation().getW());
            _isUpdateTransformFromObject = true;
        }
    }
}


void PhysicalBody::changedTransform()
{
    if (_isUpdateTransformFromObject)
    {
        btTransform transf;
        transf.setIdentity();

        glm::mat4 m;
        m = _object->getGlobalTransformMatrix();
        transf.setFromOpenGLMatrix(&m[0][0]);

        _rigidBody->setCenterOfMassTransform(transf);
        _motionState->setWorldTransform(transf);

        _rigidBody->activate();
    }
}
