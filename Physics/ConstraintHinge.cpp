#include "ConstraintHinge.hpp"

#include "../Utils/Helpers.hpp"

ConstraintHinge::ConstraintHinge(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB, btVector3 axisA, btVector3 axisB)
: Constraint(bodyA, bodyB),
_pivotA(pivotA), _pivotB(pivotB), _axisA(axisA), _axisB(axisB)
{

    updateConstraint();
}

ConstraintHinge::~ConstraintHinge()
{
    LOG_DEBUG("Hinge - Destruktor");
    safe_delete<btTypedConstraint>(_constraint);
}


void ConstraintHinge::updateConstraint()
{
    safe_delete<btTypedConstraint>(_constraint);

    _constraint = new btHingeConstraint(*(_bodyA->getRigidBody()), *(_bodyB->getRigidBody()), _pivotA, _pivotB, _axisA, _axisB);
}
