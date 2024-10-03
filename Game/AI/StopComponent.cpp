#include "StopComponent.h"

#include "AIAgentVehicle.h"

#include "../../Game/GameConfig.h"

#include "../../Graphics/Material.h"

#include "../../Physics/PhysicalBody.hpp"

#include "../../Scene/SceneManager.h"


StopComponent::StopComponent()
	: Component(CT_STOP_COMPONENT),
	_physicalBody(nullptr),
	_distanceToStop(10.0f)
{

}


void StopComponent::onAttachedToScenObject()
{
	/*_physicalBody = dynamic_cast<PhysicalBody*>(getSceneObject()->getComponent(CT_PHYSICAL_BODY));
	if (_physicalBody == nullptr)
	{
		LOG_ERROR("Cannot find PhysicalBody!");
	}*/

	//_physicalBody = getSceneObject()->getSceneManager()->getPhysicsManager()->createPhysicalBodyBox(btVector3(1.0f, 1.0f, 1.0f), 0.0f, COL_ENV, COL_BUS | COL_WHEEL);
	_physicalBody = getSceneObject()->getSceneManager()->getPhysicsManager()->createPhysicalBodyGhost(btVector3(2.0f, 2.0f, 2.0f));
	getSceneObject()->addComponent(_physicalBody);

	if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper
		SceneObject* helperSceneObject = getSceneObject()->getSceneManager()->addSceneObject("editor#StopComponent helper");
		helperSceneObject->setFlags(SOF_NOT_SELECTABLE | SOF_NOT_SELECTABLE_ON_SCENE | SOF_NOT_SERIALIZABLE);

		Material* material = new Material;
		material->shader = NOTEXTURE_MATERIAL;
		material->shininess = 96.0f;
		material->diffuseColor = glm::vec4(0.32f, 0.32f, 0.32f, 1.0f);

		Cube* cube = new Cube(1, material);
		cube->init();
		cube->setCastShadows(false);
		getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(cube, helperSceneObject);

		getSceneObject()->addChild(helperSceneObject);
	}
}


void StopComponent::update(float deltaTime)
{
	for (PhysicalBody* body : _physicalBody->getObjectsBeginCollision())
	{
		Component* aiAgentVehicleComponent = body->getSceneObject()->getComponent(CT_AI_AGENT_VEHICLE);
		if (aiAgentVehicleComponent != nullptr)
		{
			AIAgentVehicle* aiAgentVehicle = dynamic_cast<AIAgentVehicle*>(aiAgentVehicleComponent);

			LOG_DEBUG(getSceneObject()->getName() + " - Collision with agent: " + aiAgentVehicle->getSceneObject()->getName());
			aiAgentVehicle->stop(_distanceToStop);
		}
	}


	for (PhysicalBody* body : _physicalBody->getObjectsEndCollision())
	{
		LOG_DEBUG(getSceneObject()->getName() + " - Collision end with agent: " + body->getSceneObject()->getName());
	}
}
