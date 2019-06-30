#ifndef DOORCLASSIC_H_INCLUDED
#define DOORCLASSIC_H_INCLUDED


#include "Door.h"


class DoorClassic : public Door
{
    public:
        DoorClassic(RStaticModel* model, PhysicalBodyConvexHull* body, ConstraintHinge* hinge1, ConstraintHinge* hinge2, SoundComponent* openDoor, SoundComponent* closeDoor, char group = 1)
        : Door(model, body, openDoor, closeDoor, group), _hinge1(hinge1), _hinge2(hinge2)
        {
        }

        virtual ~DoorClassic()
        {
        }

        ConstraintHinge* getConstraint() { return _hinge1; }

        void open() override
        {
            //_doorOpenSound->play();
            _hinge1->getBulletConstraint()->enableAngularMotor(true, 2.1f, 0.15f);
            _hinge2->getBulletConstraint()->enableAngularMotor(true, -4.2f, 0.3f);
            _state = EDS_OPENING;

            Door::open();
        }

        void close() override
        {
            //_doorCloseSound->play();
            _hinge1->getBulletConstraint()->enableAngularMotor(true, -2.1f, 0.15f);
            _hinge2->getBulletConstraint()->enableAngularMotor(true, 4.2f, 0.3f);
            _state = EDS_CLOSING;

            Door::close();
        }

        void setLoose() override
        {
            _hinge1->getBulletConstraint()->enableAngularMotor(false, 0.0f, 0.0f);
        }

    protected:
        ConstraintHinge*         _hinge1;
        ConstraintHinge*         _hinge2;
};


#endif // DOORCLASSIC_H_INCLUDED
