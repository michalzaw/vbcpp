#ifndef CONSTRAINTHINGE_HPP_INCLUDED
#define CONSTRAINTHINGE_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "Constraint.hpp"
#include "PhysicalBody.hpp"

class ConstraintHinge : public Constraint
{
    public:
        ConstraintHinge(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB, btVector3 axisA, btVector3 axisB);
        virtual ~ConstraintHinge();

    private:
        btVector3       _pivotA;
        btVector3       _pivotB;
        btVector3       _axisA;
        btVector3       _axisB;

        void updateConstraint();
};

#endif // CONSTRAINTHINGE_HPP_INCLUDED
