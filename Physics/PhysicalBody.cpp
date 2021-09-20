#include "PhysicalBody.hpp"

#include "../Utils/Helpers.hpp"
#include "../Utils/Logger2.h"

#include "Constraint.hpp"

// =========================================
// CONSTRUCTOR & DESTRUCTOR

PhysicalBody::PhysicalBody(btScalar m, bool centerOfMassOffset, btVector3 centerOfMassOffsetValue)
: Component(CT_PHYSICAL_BODY),
_rigidBody(nullptr), _collShape(nullptr), _motionState(nullptr),
_mass(m), _position(btVector3(0,0,0)), _oldScale(btVector3(1,1,1)),
_centerOfMassOffset(centerOfMassOffset), _centerOfMassOffsetValue(centerOfMassOffsetValue),
_isUpdateTransformFromObject(true)
{

}

PhysicalBody::~PhysicalBody()
{
    LOG_INFO("Physical body - Destruktor");

}


void PhysicalBody::setVisualisationInDebugDrawingStatus(bool enabled)
{
	if (enabled)
	{
		_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() & ~(btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT));
	}
	else
	{
		_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	}
}


void PhysicalBody::update()
{
    if (_object->isActive())
    {
        if (_mass > 0)
        {
            btTransform transf;
			transf = _motionState->m_graphicsWorldTrans;

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
		_motionState->m_graphicsWorldTrans.setIdentity();

        glm::mat4 m;
        m = _object->getGlobalTransformMatrix();
		_motionState->m_graphicsWorldTrans.setFromOpenGLMatrix(&m[0][0]);

		_motionState->getWorldTransform(transf);
        _rigidBody->setCenterOfMassTransform(transf);

        for (int i = 0; i < _constraints.size(); ++i)
        {
            /*PhysicalBody* b;
            if (_constraints[i]->getBodyA() != this)
                b = _constraints[i]->getBodyA();

            else if (_constraints[i]->getBodyB() != this)
                b = _constraints[i]->getBodyB();*/
            PhysicalBody* a = _constraints[i]->getBodyA();
            PhysicalBody* b = _constraints[i]->getBodyB();

            if (a == this && b != this)
                a = b;
            else
                continue;

            glm::vec3 relativePos = getSceneObject()->transformLocalPointToGlobal(glm::vec3(a->_position.x(), a->_position.y(), a->_position.z()));

            _isUpdateTransformFromObject = false;
            a->getSceneObject()->setPosition(relativePos);
            a->getSceneObject()->setRotation(getSceneObject()->getRotation());
            _isUpdateTransformFromObject = true;
        }

        _rigidBody->activate();
    }
}
