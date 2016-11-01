#include "ConstraintHinge2.hpp"

#include "../Utils/Helpers.hpp"


ConstraintHinge2::ConstraintHinge2(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivot, btVector3 axisA, btVector3 axisB)
: Constraint(bodyA, bodyB),
_pivot(btVector3(pivot)), _axisA(btVector3(axisA)), _axisB(btVector3(axisB))
{
    std::cout << "Hinge2 Constraint - Konstruktor" << std::endl;
    UpdateConstraint();
}


ConstraintHinge2::~ConstraintHinge2()
{
    std::cout << "Hinge2 Constraint - Destruktor" << std::endl;
}


void ConstraintHinge2::setStiffness(int index, btScalar stiffness)
{
    dynamic_cast<btHinge2Constraint*>(_constraint)->setStiffness(index, stiffness);
}


void ConstraintHinge2::setDamping(int index, btScalar damping)
{
    dynamic_cast<btHinge2Constraint*>(_constraint)->setStiffness(index, damping);
}


void ConstraintHinge2::UpdateConstraint()
{
    safe_delete<btTypedConstraint>(_constraint);

    _constraint = new btHinge2Constraint(*(_bodyA->getRigidBody()), *(_bodyB->getRigidBody()), _pivot, _axisA, _axisB);
}
