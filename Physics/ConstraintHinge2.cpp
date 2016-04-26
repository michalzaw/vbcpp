#include "ConstraintHinge2.hpp"

#include "../Utils/Helpers.hpp"


ConstraintHinge2::ConstraintHinge2(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivot, btVector3 axisA, btVector3 axisB)
: Constraint(bodyA, bodyB),
_pivot(btVector3(pivot)), _axisA(btVector3(axisA)), _axisB(btVector3(axisB))
{
    UpdateConstraint();
}


ConstraintHinge2::~ConstraintHinge2()
{

}


void ConstraintHinge2::UpdateConstraint()
{
    safe_delete<btTypedConstraint>(_constraint);

    _constraint = new btHinge2Constraint(*(_bodyA->getRigidBody()), *(_bodyB->getRigidBody()), _pivot, _axisA, _axisB);
}
