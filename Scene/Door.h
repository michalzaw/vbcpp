#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include "../Utils/RModel.h"
#include "../Physics/PhysicalBodyConvexHull.hpp"

enum DoorState
{
    EDS_OPENING = 0,
    EDS_CLOSING = 1
};


class Door
{
    public:
        Door(RModel* model, PhysicalBodyConvexHull* body)
        : _doorBody(body), _doorModel(model), _state(EDS_CLOSING), _collidesWith(COL_NOTHING)
        {

        }

        virtual ~Door()
        {

        }

        virtual void open() = 0;
        virtual void close() = 0;

        PhysicalBodyConvexHull* getDoorBody() { return _doorBody; }

        RModel* getDoorModel() { return _doorModel; }



        DoorState getState() { return _state; }

        protected:
            RModel*                  _doorModel;
            PhysicalBodyConvexHull*  _doorBody;
            DoorState                _state;
            int                      _collidesWith;
};



#endif // DOOR_H_INCLUDED
