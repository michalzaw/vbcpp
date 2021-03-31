#include "DoorSE.h"

#include "../Scene/SceneManager.h"


DoorSE::DoorSE(SceneObject* doorSceneObject, SceneObject* arm1SceneObject, SceneObject* arm2SceneObject,
			   ConstraintHinge* constraintBusToArm1, ConstraintHinge* constraintBusToArm2,
			   ConstraintHinge* constraintArm1ToDoor, ConstraintHinge* constraintArm2ToDoor,
			   SoundComponent* openDoor, SoundComponent* closeDoor, RotationDir dir, char group)
	: Door(doorSceneObject, openDoor, closeDoor, group),
	_arm1SceneObject(arm1SceneObject), _arm2SceneObject(arm2SceneObject),
	_constraintBusToArm1(constraintBusToArm1), _constraintBusToArm2(constraintBusToArm2),
	_constraintArm1ToDoor(constraintArm1ToDoor), _constraintArm2ToDoor(constraintArm2ToDoor),
	_rotationDir(dir)
{

}

DoorSE::~DoorSE()
{
	SceneManager* sceneManager = _doorSceneObject->getSceneManager();

	sceneManager->getPhysicsManager()->removeConstraint(_constraintBusToArm1);
	sceneManager->getPhysicsManager()->removeConstraint(_constraintBusToArm2);
	sceneManager->getPhysicsManager()->removeConstraint(_constraintArm1ToDoor);
	sceneManager->getPhysicsManager()->removeConstraint(_constraintArm2ToDoor);

	sceneManager->removeSceneObject(_arm1SceneObject);
	sceneManager->removeSceneObject(_arm2SceneObject);
}

void DoorSE::removeObjectsAndConstraint(SceneManager* sceneManager)
{
	
}


void DoorSE::openImpl()
{
	if (_rotationDir == ERD_CCW)
		_constraintBusToArm1->getBulletConstraint()->enableAngularMotor(true, -1.7f, 2.15f);
	else
		_constraintBusToArm1->getBulletConstraint()->enableAngularMotor(true, 1.7f, 2.15f);
}


void DoorSE::closeImpl()
{
	if (_rotationDir == ERD_CCW)
		_constraintBusToArm1->getBulletConstraint()->enableAngularMotor(true, 1.7f, 2.15f);
	else
		_constraintBusToArm1->getBulletConstraint()->enableAngularMotor(true, -1.7f, 2.15f);
}


void DoorSE::setLoose()
{

}
