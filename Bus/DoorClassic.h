#ifndef DOORCLASSIC_H_INCLUDED
#define DOORCLASSIC_H_INCLUDED


#include "Door.h"

#include "../Physics/ConstraintHinge.hpp"


class DoorClassic : public Door
{
    public:
        DoorClassic(SceneObject* doorSceneObject, SceneObject* armSceneObject, ConstraintHinge* hingeBusToArm, ConstraintHinge* hingeArmToDoor,
                    SoundComponent* openDoor, SoundComponent* closeDoor, float velocity, RotationDir rotationDir = ERD_CW, char group = 1);

        virtual ~DoorClassic();

		void removeObjectsAndConstraint(SceneManager* sceneManager) override;

        void open() override;
        void close() override;
        void setLoose() override;

		ConstraintHinge* getConstraintBusToArm();
		ConstraintHinge* getConstraintArmToDoor();

    protected:
		SceneObject*			_armSceneObject;
        ConstraintHinge*        _hingeBusToArm;
        ConstraintHinge*        _hingeArmToDoor;
        RotationDir             _rotationDir;

        float                   _velocity;
};


#endif // DOORCLASSIC_H_INCLUDED
