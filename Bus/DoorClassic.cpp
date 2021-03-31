#include "DoorClassic.h"

#include "../Scene/SceneManager.h"


DoorClassic::DoorClassic(SceneObject* doorSceneObject, SceneObject* armSceneObject, ConstraintHinge* hingeBusToArm, ConstraintHinge* hingeArmToDoor,
						 SoundComponent* openDoor, SoundComponent* closeDoor, float velocity, RotationDir rotationDir, char group)
    : Door(doorSceneObject, openDoor, closeDoor, group), _armSceneObject(armSceneObject), _hingeBusToArm(hingeBusToArm), _hingeArmToDoor(hingeArmToDoor),
	_rotationDir(rotationDir), _velocity(velocity)
{
}


DoorClassic::~DoorClassic()
{
	SceneManager* sceneManager = _doorSceneObject->getSceneManager();

	sceneManager->getPhysicsManager()->removeConstraint(_hingeBusToArm);
	sceneManager->getPhysicsManager()->removeConstraint(_hingeArmToDoor);

	sceneManager->removeSceneObject(_armSceneObject);
}


void DoorClassic::removeObjectsAndConstraint(SceneManager* sceneManager)
{

}


void DoorClassic::openImpl()
{
    int direction = 0;
    if (_rotationDir == ERD_CW)
        direction = 1;
    else if (_rotationDir == ERD_CCW)
        direction = -1;

    _hingeBusToArm->getBulletConstraint()->enableAngularMotor(true, 2.1f * direction * _velocity, 0.15f);
    _hingeArmToDoor->getBulletConstraint()->enableAngularMotor(true, -4.2f * direction * _velocity, 0.3f);
    _state = EDS_OPENING;
}


void DoorClassic::closeImpl()
{
    int direction = 0;
    if (_rotationDir == ERD_CW)
        direction = 1;
    else if (_rotationDir == ERD_CCW)
        direction = -1;

    _hingeBusToArm->getBulletConstraint()->enableAngularMotor(true, -2.1f * direction * _velocity, 0.15f);
    _hingeArmToDoor->getBulletConstraint()->enableAngularMotor(true, 4.2f * direction * _velocity, 0.3f);
    _state = EDS_CLOSING;
}


void DoorClassic::setLoose()
{
    _hingeBusToArm->getBulletConstraint()->enableAngularMotor(false, 0.0f, 0.0f);
    _hingeArmToDoor->getBulletConstraint()->enableAngularMotor(false, 0.0f, 0.0f);
}


ConstraintHinge* DoorClassic::getConstraintBusToArm()
{
	return _hingeBusToArm;
}


ConstraintHinge* DoorClassic::getConstraintArmToDoor()
{
	return _hingeArmToDoor;
}
