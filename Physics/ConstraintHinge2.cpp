#include "ConstraintHinge2.hpp"

#include "../Utils/Helpers.hpp"


ConstraintHinge2::ConstraintHinge2(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivot, btVector3 axisA, btVector3 axisB)
: Constraint(bodyA, bodyB),
_pivot(btVector3(pivot)), _axisA(btVector3(axisA)), _axisB(btVector3(axisB))
{
    std::cout << "Hinge2 Constraint - Konstruktor" << std::endl;
    updateConstraint();
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
    dynamic_cast<btHinge2Constraint*>(_constraint)->setDamping(index, damping);
}


void ConstraintHinge2::updateConstraint()
{
    safe_delete<btTypedConstraint>(_constraint);

    glm::vec3 newPivot = _bodyA->getSceneObject()->transformLocalPointToGlobal(glm::vec3(_pivot.x(), _pivot.y(), _pivot.z()));
    glm::vec3 newAxisA = _bodyA->getSceneObject()->transformLocalVectorToGlobal(glm::vec3(_axisA.x(), _axisA.y(), _axisA.z()));
    glm::vec3 newAxisB = _bodyA->getSceneObject()->transformLocalVectorToGlobal(glm::vec3(_axisB.x(), _axisB.y(), _axisB.z()));
    btVector3 newBtPivot(newPivot.x, newPivot.y, newPivot.z);
    btVector3 newBtAxisA(newAxisA.x, newAxisA.y, newAxisA.z);
    btVector3 newBtAxisB(newAxisB.x, newAxisB.y, newAxisB.z);

    _constraint = new btHinge2Constraint(*(_bodyA->getRigidBody()), *(_bodyB->getRigidBody()), newBtPivot, newBtAxisA, newBtAxisB);
}
