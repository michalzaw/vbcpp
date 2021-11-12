#include "Constraint.hpp"

#include "../Utils/Helpers.hpp"
#include "../Utils/Logger.h"

Constraint::Constraint(PhysicalBody* bodyA, PhysicalBody* bodyB)
: _bodyA(bodyA), _bodyB(bodyB), _constraint(0)
{
    LOG_INFO("Hinge - Konstruktor");

    bodyA->addConstraint(this);
    bodyB->addConstraint(this);

    glm::vec3 posA = bodyA->getSceneObject()->getPosition();
    glm::vec3 posB = bodyB->getSceneObject()->getPosition();
    bodyB->setDefaultPosition(btVector3(posB.x - posA.x, posB.y - posA.y, posB.z - posA.z));


    glm::vec3 relativePos = bodyA->getSceneObject()->transformLocalPointToGlobal(bodyB->getSceneObject()->getPosition());

    bodyB->getSceneObject()->setPosition(relativePos);
    bodyB->getSceneObject()->setRotation(bodyB->getSceneObject()->getRotation());
}


Constraint::~Constraint()
{
    safe_delete<btTypedConstraint>(_constraint);

	_bodyA->removeConstraint(this);
	_bodyB->removeConstraint(this);
}

