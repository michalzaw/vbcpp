#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include "SceneManager.h"
#include "../Physics/PhysicsManager.hpp"

enum DoorState
{
    EDS_OPENING = 0,
    EDS_CLOSING = 1
};


class Door
{
    public:
        Door(RModel* model, PhysicalBodyConvexHull* body, ConstraintHinge* hinge)
        : _hinge(hinge), _body(body), _model(model)
        {

        }

        virtual ~Door()
        {

        }

        void open()
        {
            _hinge->getBulletConstraint()->enableAngularMotor(true, -1.9f, 1.15f);
            _state = EDS_OPENING;
        }

        void close()
        {
            _hinge->getBulletConstraint()->enableAngularMotor(true, 1.9f, 1.15f);
            _state = EDS_CLOSING;
        }

        PhysicalBodyConvexHull* getPhysicalBody() { return _body; }

        RModel* getModel() { return _model; }

        ConstraintHinge* getConstraint() { return _hinge; }

        DoorState getState() { return _state; }

        protected:
            PhysicalBodyConvexHull*  _body;
            RModel*                  _model;
            ConstraintHinge*         _hinge;
            DoorState                _state;
};



#endif // DOOR_H_INCLUDED
