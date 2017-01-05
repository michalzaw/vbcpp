#ifndef DOORSIMPLE_H_INCLUDED
#define DOORSIMPLE_H_INCLUDED

#include "Door.h"
#include "../Physics/ConstraintHinge.hpp"

class DoorSimple : virtual public Door
{
    public:
        DoorSimple(RModel* model, PhysicalBodyConvexHull* body, ConstraintHinge* hinge, char group = 1)
        : Door(model, body, group), _hinge(hinge)
        {
            close();
        }

        virtual ~DoorSimple()
        {

        }

        ConstraintHinge* getConstraint() { return _hinge; }

        void open()
        {
            _hinge->getBulletConstraint()->enableAngularMotor(true, -1.9f, 0.15f);
            _state = EDS_OPENING;
        }

        void close()
        {
            _hinge->getBulletConstraint()->enableAngularMotor(true, 1.9f, 0.15f);
            _state = EDS_CLOSING;
        }

    protected:
        ConstraintHinge*         _hinge;
};


#endif // DOORSIMPLE_H_INCLUDED
