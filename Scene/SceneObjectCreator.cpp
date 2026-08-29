#include "SceneObjectCreator.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Utils/RStaticModel.h"


namespace SceneObjectCreator
{
	SceneObject* createSceneObject(RStaticModel* model, StaticModelNode* modelNode, SceneManager* sceneManager)
	{
		SceneObject* sceneObject = sceneManager->addSceneObject(modelNode->name);
		sceneObject->setPosition(modelNode->transform.getPosition());
		sceneObject->setRotation(modelNode->transform.getRotation());
		sceneObject->setScale(modelNode->transform.getScale());

		RenderObject* renderObject = sceneManager->getGraphicsManager()->addRenderObject(new RenderObject(model, modelNode, true, true), sceneObject);

		for (StaticModelNode* childNode : modelNode->children)
		{
			SceneObject* childSceneObject = createSceneObject(model, childNode, sceneManager);
			sceneObject->addChild(childSceneObject);
		}

		return sceneObject;
	}

	SceneObject* createHierarchicalSceneObjectWithModel(RStaticModel* model, SceneManager* sceneManager)
	{
		return createSceneObject(model, model->getRootNode(), sceneManager);
	}

}
