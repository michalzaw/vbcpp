#ifndef CONSTRAINTBALL_H_INCLUDED
#define CONSTRAINTBALL_H_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "Constraint.hpp"
#include "PhysicalBody.hpp"

class ConstraintBall : public Constraint
{
    public:
        ConstraintBall(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB);
        virtual ~ConstraintBall();

        btPoint2PointConstraint* getBulletConstraint() { return dynamic_cast<btPoint2PointConstraint*>(_constraint); }

    private:
        btVector3       _pivotA;
        btVector3       _pivotB;

        void updateConstraint();
};

#endif // CONSTRAINTBALL_H_INCLUDED
