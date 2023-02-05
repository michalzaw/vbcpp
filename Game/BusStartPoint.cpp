#include "BusStartPoint.h"

#include "GameConfig.h"

#include "../Scene/SceneManager.h"


BusStartPoint::BusStartPoint(const std::string& name)
	: Component(CT_BUS_START_POINT),
	_name(name)
{

}


void BusStartPoint::onAttachedToScenObject()
{
	if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper
		SceneObject* helperSceneObject = getSceneObject()->getSceneManager()->addSceneObject("editor#BusStartPoint helper");
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
