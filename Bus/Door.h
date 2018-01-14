#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include "../Utils/RModel.h"
#include "../Physics/PhysicalBodyConvexHull.hpp"
#include "../Scene/SoundComponent.h"

enum DoorState
{
    EDS_OPENING = 0,
    EDS_CLOSING,
    EDS_LOOSE
};


class Door
{
    public:
        Door(RModel* model, PhysicalBodyConvexHull* body, SoundComponent* openDoor, SoundComponent* closeDoor, char group = 1)
        : _group(group), _doorModel(model), _doorBody(body), _state(EDS_CLOSING), _collidesWith(COL_TERRAIN), _doorOpenSound(openDoor), _doorCloseSound(closeDoor)
        {
        }

        virtual ~Door()
        {
        }

        virtual void open() { _doorOpenSound->play(); }
        virtual void close() { _doorCloseSound->play(); }
        virtual void setLoose() = 0;


        void playOpenSound() { _doorOpenSound->play(); }
        void playCloseSound() { _doorCloseSound->play(); }

        PhysicalBodyConvexHull* getDoorBody() { return _doorBody; }

        const RModel* getDoorModel() const { return _doorModel; }

        char getGroup() { return _group; }

        DoorState getState() { return _state; }

        protected:
            char                     _group;
            RModel*                  _doorModel;
            PhysicalBodyConvexHull*  _doorBody;
            DoorState                _state;
            int                      _collidesWith;
            SoundComponent*          _doorOpenSound;
            SoundComponent*          _doorCloseSound;
};



#endif // DOOR_H_INCLUDED
