#include "ConstraintBall.h"

#include "../Utils/Helpers.hpp"

ConstraintBall::ConstraintBall(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB)
: Constraint(bodyA, bodyB),
_pivotA(pivotA), _pivotB(pivotB)
{
    std::cout << "Ball Constraint - Konstruktor" << std::endl;
    updateConstraint();
}


ConstraintBall::~ConstraintBall()
{
    std::cout << "Ball Constraint - Destruktor" << std::endl;
}


void ConstraintBall::updateConstraint()
{
    safe_delete<btTypedConstraint>(_constraint);

    glm::vec3 newPivotA = _bodyA->getSceneObject()->transformLocalPointToGlobal(glm::vec3(_pivotA.x(), _pivotA.y(), _pivotA.z()));
    glm::vec3 newPivotB = _bodyB->getSceneObject()->transformLocalVectorToGlobal(glm::vec3(_pivotB.x(), _pivotB.y(), _pivotB.z()));
    btVector3 newBtPivotA = btVector3(newPivotA.x, newPivotA.y, newPivotA.z);
    btVector3 newBtPivotB = btVector3(newPivotB.x, newPivotB.y, newPivotB.z);

    //_constraint = new btHingeConstraint(*(_bodyA->getRigidBody()), *(_bodyB->getRigidBody()), _pivotA, _pivotB, _axisA, _axisB);
    _constraint = new btPoint2PointConstraint(*(_bodyA->getRigidBody()), *(_bodyB->getRigidBody()), newBtPivotA, newBtPivotB);
}
