#include "RObjectLoader.h"

#include <sstream>
#include <cstdlib>

#include "Logger.h"
#include "Helpers.hpp"
#include "Strings.h"
#include "XmlUtils.h"

#include "tinyxml2.h"
using namespace tinyxml2;

#include "../Scene/SceneManager.h"

#include "../Utils/ResourceManager.h"


void RObjectLoader::loadComponents(XMLElement* objectElement, RObject* object)
{
	XMLElement* componentsElement = objectElement->FirstChildElement("Components");
	if (componentsElement == nullptr)
	{
		Logger::error("Components element not found!");
		return;
	}

	XMLElement* componentElement = componentsElement->FirstChildElement("Component");
	while (componentElement != nullptr)
	{
		std::string componentType = componentElement->Attribute("type");
		Logger::info("Component: " + componentType);

		object->getComponents().push_back(std::unordered_map<std::string, std::string>());
		int componentIndex = object->getComponents().size() - 1;
		object->getComponents()[componentIndex]["type"] = componentType;

		if (componentType == "render")
		{
			loadRenderComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "physics")
		{
			loadPhysicsComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "tree")
		{
			loadTreeComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "sound")
		{
			loadSoundComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "bus-stop")
		{
			loadBusStopComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "environmentCapture")
		{
			loadEnvironmentCaptureComponent(componentElement, object, componentIndex);
		}

		componentElement = componentElement->NextSiblingElement("Component");
	}

}


void RObjectLoader::loadRenderComponent(XMLElement* componentElement, RObject* object, int componentIndex)
{
	object->getComponents()[componentIndex]["model"] = componentElement->Attribute("model");

	object->getComponents()[componentIndex]["dynamic"] = XmlUtils::getAttributeStringOptional(componentElement, "dynamic");
	object->getComponents()[componentIndex]["normalsSmoothing"] = XmlUtils::getAttributeStringOptional(componentElement, "normalsSmoothing", "true");
}


void RObjectLoader::loadPhysicsComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	std::string bodyType(componentElement->Attribute("body"));
	object->getComponents()[componentIndex]["body"] = bodyType;

	if (bodyType == "box")
	{
		object->getComponents()[componentIndex]["halfExtents"] = componentElement->Attribute("halfExtents");
		object->getComponents()[componentIndex]["mass"] = componentElement->Attribute("mass");
	}
	else if (bodyType == "dynamic")
	{
		object->getComponents()[componentIndex]["mass"] = componentElement->Attribute("mass");
	}
	else if (bodyType == "static")
	{
		// not contains additional parameters
	}
	else if (bodyType == "bvh")
	{
		// not contains additional parameters
	}
}


void RObjectLoader::loadTreeComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	// not contains additional parameters
}


void RObjectLoader::loadSoundComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	object->getComponents()[componentIndex]["file"] = componentElement->Attribute("file");
	object->getComponents()[componentIndex]["looping"] = componentElement->Attribute("looping");

	// if set in config file - we set play distance accordingly (otherwise - play distance is 10.0f by default)
	std::string playDistance = XmlUtils::getAttributeStringOptional(componentElement, "playDistance", "10.0");
	Logger::info("Sound play distance: " + playDistance);
	object->getComponents()[componentIndex]["playDistance"] = playDistance;

	// if set in config file - we set volume accordingly (otherwise - volume is 1.0f by default)
	std::string volume = XmlUtils::getAttributeStringOptional(componentElement, "volume", "1.0");
	Logger::info("Sound volume: " + volume);
	object->getComponents()[componentIndex]["volume"] = volume;

	std::string position = XmlUtils::getAttributeStringOptional(componentElement, "position", "0,0,0");
	Logger::info("Sound position offset: " + position);
	object->getComponents()[componentIndex]["position"] = position;
}


void RObjectLoader::loadBusStopComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	// not contains additional parameters
}


void RObjectLoader::loadEnvironmentCaptureComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	object->getComponents()[componentIndex]["textures"] = componentElement->Attribute("textures");
}


RObject* RObjectLoader::loadObject(std::string dirPath)
{
	std::string fullPath = dirPath + OBJECT_FILE_NAME;

	XMLDocument doc;
	XMLError result = doc.LoadFile(fullPath.c_str());
	if (result != XML_SUCCESS)
	{
		Logger::error("Cannot read xml file: " + fullPath + "! Result: " + toString(result));
	}
	
	// Search for main element - Object
	XMLElement* objElement = doc.FirstChildElement("Object");
	if (objElement == nullptr)
	{
		Logger::error("Object element not found!");
		return nullptr;
	}

	XMLElement* objDesc = objElement->FirstChildElement("Description");
	if (objDesc == nullptr)
	{
		Logger::error("Description element not found!");
		return nullptr;
	}

	// Load file description
	std::string author(objDesc->Attribute("author"));
	std::string objectName(objDesc->Attribute("name"));
	std::string comment(objDesc->Attribute("comment"));

	Logger::info("*** OBJECT DATA ***");
	Logger::info("Author: " + author);
	Logger::info("Name: " + objectName);
	Logger::info("Comment: " + comment);


	RObject* object = new RObject(dirPath, author, objectName, comment);


	loadComponents(objElement, object);


	return object;
}


SceneObject* RObjectLoader::createSceneObjectFromRObject(RObject* objectDefinition, std::string name,
														 glm::vec3 position, glm::vec3 rotation, SceneManager* sceneManager)
{
	std::string objectDirPath = objectDefinition->getPath();

	SceneObject* sceneObject = sceneManager->addSceneObject(name, objectDefinition);
	sceneObject->setPosition(position);
	sceneObject->setRotation(degToRad(rotation.x), degToRad(rotation.y), degToRad(rotation.z));

	RStaticModel* model = nullptr;

	std::vector<std::unordered_map<std::string, std::string>>& components = objectDefinition->getComponents();

	for (int i = 0; i < components.size(); ++i)
	{
		std::string componentType = components[i]["type"];

		if (componentType == "render")
		{
			std::string modelFile = components[i]["model"];

			std::string modelPath = objectDirPath + modelFile;

			model = ResourceManager::getInstance().loadModel(modelPath, objectDirPath, toBool(components[i]["normalsSmoothing"]));
			RenderObject* renderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(model), sceneObject);
			renderObject->setIsDynamicObject(toBool(components[i]["dynamic"]));
		}
		else if (componentType == "physics")
		{
			std::string bodyType = components[i]["body"];

			PhysicsManager* physicsManager = sceneManager->getPhysicsManager();
			PhysicalBody* physicalBody;
			if (bodyType == "box")
			{
				int collidesWith = COL_TERRAIN | COL_BUS | COL_ENV;
				float halfExtents = toFloat(components[i]["halfExtents"]);
				float mass = toFloat(components[i]["mass"]);

				physicalBody = physicsManager->createPhysicalBodyBox(btVector3(halfExtents, halfExtents, halfExtents), mass, COL_ENV, collidesWith);
				sceneObject->addComponent(physicalBody);
			}
			else if (bodyType == "dynamic")
			{
				Logger::info("- Creating dynamic Convex Hull collision shape");

				float mass = toFloat(components[i]["mass"]);
				int collidesWith = COL_TERRAIN | COL_WHEEL | COL_BUS | COL_DOOR | COL_ENV;

				PhysicalBodyConvexHull* physicalBody = physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), mass,
					COL_ENV, collidesWith);

				sceneObject->addComponent(physicalBody);
			}
			else if (bodyType == "static")
			{
				Logger::info("- Creating static Convex Hull collision shape");

				int collidesWith = COL_WHEEL | COL_BUS | COL_DOOR | COL_ENV;

				PhysicalBodyConvexHull* physicalBody = physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), 0,
					COL_ENV, collidesWith);
				//terrainMesh->setRestitution(0.9f);
				//terrainMesh->getRigidBody()->setFriction(1.0f);
				sceneObject->addComponent(physicalBody);
			}
			else if (bodyType == "bvh")
			{
				Logger::info("- Creating BVH Triangle Mesh collision shape");

				int collidesWith = COL_WHEEL | COL_BUS | COL_DOOR | COL_ENV;

				PhysicalBodyBvtTriangleMesh* physicalBody = physicsManager->createPhysicalBodyBvtTriangleMesh(model, COL_ENV, collidesWith);
				//terrainMesh->setRestitution(0.9f);
				//terrainMesh->getRigidBody()->setFriction(1.0f);
				sceneObject->addComponent(physicalBody);
			}
		}
		else if (componentType == "tree")
		{
			TreeComponent* component = new TreeComponent;
			sceneObject->addComponent(component);
		}
		else if (componentType == "sound")
		{
			std::string soundFile = components[i]["file"];
			bool looping = toBool(components[i]["looping"]);
			float playDistance = toFloat(components[i]["playDistance"]);
			float volume = toFloat(components[i]["volume"]);
			glm::vec3 soundPosition = XMLstringToVec3(components[i]["position"].c_str());

			std::string soundPath = objectDirPath + soundFile;

			RSound* soundResource = ResourceManager::getInstance().loadSound(soundPath);
			SoundComponent* sound = new SoundComponent(soundResource, EST_AMBIENT, looping);
			sceneManager->getSoundManager()->addSoundComponent(sound);

			sound->setPlayDistance(playDistance);
			sound->setGain(volume);
			sound->setPosition(soundPosition);

			sceneObject->addComponent(sound);
		}
		else if (componentType == "bus-stop")
		{
			BusStopComponent* component = BusStopSystem::getInstance().addBusStopComponent("Przystanek");
			sceneObject->addComponent(component);
		}
		else if (componentType == "environmentCapture")
		{
			std::string textures = components[i]["textures"];
			std::vector<std::string> t = split(textures, ',');

			RTextureCubeMap* cubeMap = ResourceManager::getInstance().loadTextureCubeMap(&t[0]);
			EnvironmentCaptureComponent* component = GraphicsManager::getInstance().addEnvironmentCaptureComponent(cubeMap);
			sceneObject->addComponent(component);
		}
	}

	return sceneObject;
}
