#ifndef DOORSIMPLE_H_INCLUDED
#define DOORSIMPLE_H_INCLUDED

#include "Door.h"
#include "../Physics/ConstraintHinge.hpp"

class DoorSimple : virtual public Door
{
    public:
        DoorSimple(RStaticModel* model, PhysicalBodyConvexHull* body, ConstraintHinge* hinge, SoundComponent* openDoor, SoundComponent* closeDoor, char group = 1)
        : Door(model, body, openDoor, closeDoor, group), _hinge(hinge)
        {
        }

        virtual ~DoorSimple()
        {
        }

        ConstraintHinge* getConstraint() { return _hinge; }

        void open() override
        {
            //_doorOpenSound->play();
            _hinge->getBulletConstraint()->enableAngularMotor(true, -2.1f, 0.15f);
            _state = EDS_OPENING;

            Door::open();
        }

        void close() override
        {
            //_doorCloseSound->play();
            _hinge->getBulletConstraint()->enableAngularMotor(true, 2.1f, 0.15f);
            _state = EDS_CLOSING;

            Door::close();
        }

        void setLoose() override
        {
            _hinge->getBulletConstraint()->enableAngularMotor(false, 0.0f, 0.0f);
        }

    protected:
        ConstraintHinge*         _hinge;
};


#endif // DOORSIMPLE_H_INCLUDED
