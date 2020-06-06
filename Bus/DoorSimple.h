#ifndef DOORSIMPLE_H_INCLUDED
#define DOORSIMPLE_H_INCLUDED

#include "Door.h"

#include "../Physics/ConstraintHinge.hpp"

class DoorSimple : public Door
{
    public:
		DoorSimple(SceneObject* doorSceneObject, ConstraintHinge* hinge, SoundComponent* openDoor, SoundComponent* closeDoor, char group = 1);

		virtual ~DoorSimple();

		void removeObjectsAndConstraint(SceneManager* sceneManager) override;

		void open() override;
		void close() override;
		void setLoose() override;

		ConstraintHinge* getConstraint() { return _hingeConstraint; }

    protected:
        ConstraintHinge*         _hingeConstraint;
};


#endif // DOORSIMPLE_H_INCLUDED
