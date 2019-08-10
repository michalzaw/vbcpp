#include "SceneSaver.h"

#include "SceneManager.h"

#include "../Game/Directories.h"

#include "../Utils/Strings.h"


SceneSaver::SceneSaver(SceneManager* sceneManager)
	: _sceneManager(sceneManager)
{

}


void SceneSaver::saveMap(std::string name)
{
	std::string dirPath = GameDirectories::MAPS + name + "/";
	std::string fullPath = dirPath + MAP_FILE_NAME;

	XMLDocument doc;

	XMLDeclaration* declaration = doc.NewDeclaration();
	doc.InsertFirstChild(declaration);

	XMLNode* rootNode = doc.NewElement("Scene");
	doc.InsertEndChild(rootNode);

	XMLElement* objectsElement = doc.NewElement("Objects");
	rootNode->InsertEndChild(objectsElement);

	for (SceneObject* sceneObject : _sceneManager->getSceneObjects())
	{
		RObject* objectDefinition = sceneObject->getObjectDefinition();
		if (objectDefinition != nullptr)
		{
			XMLElement* objectElement = doc.NewElement("Object");

			//objectElement->SetAttribute("name", objectDefinition->getName().c_str());
			objectElement->SetAttribute("name", sceneObject->getName().c_str());
			objectElement->SetAttribute("position", vec3ToString(sceneObject->getPosition()).c_str());
			glm::vec3 rotation = glm::vec3(radToDeg(sceneObject->getRotation().x),
										   radToDeg(sceneObject->getRotation().y),
										   radToDeg(sceneObject->getRotation().z));
			objectElement->SetAttribute("rotation", vec3ToString(rotation).c_str());

			objectsElement->InsertEndChild(objectElement);
		}
	}

	doc.SaveFile(fullPath.c_str());
}
