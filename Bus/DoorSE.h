#ifndef DOORSE_H_INCLUDED
#define DOORSE_H_INCLUDED

#include "Door.h"

#include "../Physics/ConstraintHinge.hpp"

class DoorSE : public Door
{
    public:
		DoorSE(SceneObject* doorSceneObject, SceneObject* arm1SceneObject, SceneObject* arm2SceneObject,
			   ConstraintHinge* constraintBusToArm1, ConstraintHinge* constraintBusToArm2,
			   ConstraintHinge* constraintArm1ToDoor, ConstraintHinge* constraintArm2ToDoor,
			   SoundComponent* openDoor, SoundComponent* closeDoor, RotationDir dir = ERD_CCW, char group = 1);

		virtual ~DoorSE();

		void removeObjectsAndConstraint(SceneManager* sceneManager) override;

		void setLoose() override;

    protected:
		SceneObject*			_arm1SceneObject;
		SceneObject*			_arm2SceneObject;
		ConstraintHinge*		_constraintBusToArm1;
		ConstraintHinge*		_constraintBusToArm2;
		ConstraintHinge*		_constraintArm1ToDoor;
		ConstraintHinge*		_constraintArm2ToDoor;

		RotationDir             _rotationDir;

		void openImpl() override;
		void closeImpl() override;

};

#endif // DOORSE_H_INCLUDED
