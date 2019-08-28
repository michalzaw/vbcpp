#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include "../Utils/RStaticModel.h"
#include "../Physics/PhysicalBodyConvexHull.hpp"
#include "../Scene/SoundComponent.h"

enum DoorState
{
    EDS_OPENING = 0,
    EDS_CLOSING,
    EDS_LOOSE
};

enum RotationDir
{
    ERD_CCW = 0,
    ERD_CW
};


class Door
{
    public:
		Door(SceneObject* doorSceneObject, SoundComponent* openDoor, SoundComponent* closeDoor, char group = 1);

		virtual ~Door();

		virtual void removeObjectsAndConstraint(SceneManager* sceneManager);

        virtual void open()
		{
			_state = EDS_OPENING;
			_doorOpenSound->play();
		}

        virtual void close()
		{
			_state = EDS_CLOSING;
			_doorCloseSound->play();
		}

        virtual void setLoose() = 0;

        void playOpenSound() { _doorOpenSound->play(); }
        void playCloseSound() { _doorCloseSound->play(); }

        char getGroup() { return _group; }

        DoorState getState() { return _state; }

		SceneObject* getSceneObject() { return _doorSceneObject; }

	protected:
        char                     _group;
        DoorState                _state;
		SceneObject*			 _doorSceneObject;
        SoundComponent*          _doorOpenSound;
        SoundComponent*          _doorCloseSound;
};



#endif // DOOR_H_INCLUDED
