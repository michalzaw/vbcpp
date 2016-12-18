#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include "SceneManager.h"
#include "../Physics/PhysicsManager.hpp"

enum DoorState
{
    EDS_OPENING = 0,
    EDS_CLOSING = 1
};


struct Door
{
    PhysicalBodyConvexHull*  body;
    RModel*                  model;
    ConstraintHinge*         hinge;
    DoorState                state;

    void open()
    {
        hinge->getBulletConstraint()->enableAngularMotor(true, -1.9f, 1.15f);
        state = EDS_OPENING;
    }

    void close()
    {
        hinge->getBulletConstraint()->enableAngularMotor(true, 1.9f, 1.15f);
        state = EDS_CLOSING;
    }
};



#endif // DOOR_H_INCLUDED
