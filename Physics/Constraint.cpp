#include "Constraint.hpp"

#include "../Utils/Helpers.hpp"

Constraint::Constraint(PhysicalBody* bodyA, PhysicalBody* bodyB)
: _bodyA(bodyA), _bodyB(bodyB), _constraint(0)
{
    printf("Hinge - Konstruktor");

}


Constraint::~Constraint()
{
    safe_delete<btTypedConstraint>(_constraint);
}

