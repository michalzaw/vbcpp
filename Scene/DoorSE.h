#ifndef DOORSE_H_INCLUDED
#define DOORSE_H_INCLUDED

#include "Door.h"

class DoorSE : virtual public Door
{
    public:
        DoorSE(RModel* model, PhysicalBodyConvexHull* body,
               RModel* armModel, PhysicalBodyConvexHull* armBody, ConstraintHinge* constraint1, ConstraintHinge* constraint2)
        : Door(model, body),
        _armModel(armModel), _armBody(armBody), _constraint1(constraint1), _constraint2(constraint2)
        {
            //close();
        }

        virtual ~DoorSE()
        {

        }

        void open()
        {
            _constraint1->getBulletConstraint()->enableAngularMotor(true, -1.9f, 2.15f);
            _state = EDS_OPENING;
        }

        void close()
        {
            _constraint1->getBulletConstraint()->enableAngularMotor(true, 1.9f, 2.15f);
            _state = EDS_CLOSING;
        }

        RModel* getArmModel() { return _armModel; }
        PhysicalBodyConvexHull* getArmBody() { return _armBody; }

        ConstraintHinge* getConstraint1() { return _constraint1; }

    protected:
        RModel*                 _armModel;
        PhysicalBodyConvexHull* _armBody;
        ConstraintHinge*        _constraint1;   // bus - arm
        ConstraintHinge*        _constraint2;   // arm - door
};

#endif // DOORSE_H_INCLUDED
