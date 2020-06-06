#include "DoorSimple.h"

#include "../Scene/SceneManager.h"


DoorSimple::DoorSimple(SceneObject* doorSceneObject, ConstraintHinge* hinge, SoundComponent* openDoor, SoundComponent* closeDoor, char group)
	: Door(doorSceneObject, openDoor, closeDoor, group), _hingeConstraint(hinge)
{
}


DoorSimple::~DoorSimple()
{
	SceneManager* sceneManager = _doorSceneObject->getSceneManager();

	sceneManager->getPhysicsManager()->removeConstraint(_hingeConstraint);
}


void DoorSimple::removeObjectsAndConstraint(SceneManager* sceneManager)
{

}


void DoorSimple::open()
{
	_hingeConstraint->getBulletConstraint()->enableAngularMotor(true, -2.1f, 0.15f);

	Door::open();
}


void DoorSimple::close()
{
	_hingeConstraint->getBulletConstraint()->enableAngularMotor(true, 2.1f, 0.15f);

	Door::close();
}


void DoorSimple::setLoose()
{
	_hingeConstraint->getBulletConstraint()->enableAngularMotor(false, 0.0f, 0.0f);
}
