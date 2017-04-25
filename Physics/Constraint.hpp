#ifndef CONSTRAINT_HPP_INCLUDED
#define CONSTRAINT_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "PhysicalBody.hpp"

class Constraint
{
    public:
        Constraint(PhysicalBody* bodyA, PhysicalBody* bodyB);
        virtual ~Constraint();

        btTypedConstraint* getBulletConstraint() { return _constraint; }

        PhysicalBody* getBodyA() { return _bodyA; }
        PhysicalBody* getBodyB() { return _bodyB; }

    protected:
        PhysicalBody*   _bodyA;
        PhysicalBody*   _bodyB;
        btTypedConstraint*  _constraint;

    virtual void updateConstraint() { }
};

#endif // CONSTRAINT_HPP_INCLUDED
