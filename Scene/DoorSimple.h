#ifndef DOORSIMPLE_H_INCLUDED
#define DOORSIMPLE_H_INCLUDED

#include "Door.h"
#include "../Physics/ConstraintHinge.hpp"

class DoorSimple : virtual public Door
{
    public:
        DoorSimple(RModel* model, PhysicalBodyConvexHull* body, ConstraintHinge* hinge, SoundComponent* openDoor, SoundComponent* closeDoor, char group = 1)
        : Door(model, body, openDoor, closeDoor, group), _hinge(hinge)
        {
            close();
        }

        virtual ~DoorSimple()
        {

        }

        ConstraintHinge* getConstraint() { return _hinge; }

        void open()
        {
            //_doorOpenSound->play();
            _hinge->getBulletConstraint()->enableAngularMotor(true, -2.1f, 0.15f);
            _state = EDS_OPENING;
        }

        void close()
        {
            //_doorCloseSound->play();
            _hinge->getBulletConstraint()->enableAngularMotor(true, 2.1f, 0.15f);
            _state = EDS_CLOSING;
        }

    protected:
        ConstraintHinge*         _hinge;
};


#endif // DOORSIMPLE_H_INCLUDED
