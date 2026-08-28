#ifndef SCENEOBJECTCREATOR_H_INCLUDED
#define SCENEOBJECTCREATOR_H_INCLUDED


class SceneObject;
class SceneManager;
class RStaticModel;


namespace SceneObjectCreator
{
	SceneObject* createHierarchicalSceneObjectWithModel(RStaticModel* model, SceneManager* sceneManager);

}


#endif // SCENEOBJECTCREATOR_H_INCLUDED