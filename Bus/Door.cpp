#include "Door.h"

#include "../Scene/SceneManager.h"


Door::Door(SceneObject* doorSceneObject, SoundComponent* openDoor, SoundComponent* closeDoor, char group)
	: _group(group), _state(EDS_CLOSING), _doorSceneObject(doorSceneObject), _doorOpenSound(openDoor), _doorCloseSound(closeDoor), _blocked(false)
{

}


Door::~Door()
{
	SceneManager* sceneManager = _doorSceneObject->getSceneManager();

	sceneManager->removeSceneObject(_doorSceneObject);
}


void Door::removeObjectsAndConstraint(SceneManager* sceneManager)
{

}
