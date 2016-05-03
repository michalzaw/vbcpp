#ifndef CONSTRAINTHINGE2_HPP_INCLUDED
#define CONSTRAINTHINGE2_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "Constraint.hpp"
#include "PhysicalBody.hpp"

class ConstraintHinge2 : public Constraint
{
    public:
        ConstraintHinge2(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivot, btVector3 axisA, btVector3 axisB);
        virtual ~ConstraintHinge2();

        btHinge2Constraint* getBulletConstraint() { return dynamic_cast<btHinge2Constraint*>(_constraint); }

    private:
        btVector3       _pivot;
        btVector3       _axisA;
        btVector3       _axisB;

        void UpdateConstraint();
};


#endif // CONSTRAINTHINGE2_HPP_INCLUDED
