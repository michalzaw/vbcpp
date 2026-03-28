#include "RObjectLoader.h"

#include <sstream>
#include <cstdlib>

#include "Logger.h"
#include "Helpers.hpp"
#include "Strings.h"
#include "XmlUtils.h"

#include "tinyxml2.h"
using namespace tinyxml2;

#include "../Bus/BusLoader.h"

#include "../Game/AI/AIAgent.h"
#include "../Game/AI/AIAgentPhysicalVechicle.h"
#include "../Game/AI/AIAgentVehicle.h"
#include "../Game/AI/TrafficLightsComponent.h"
#include "../Game/Directories.h"
#include "../Game/GameLogicSystem.h"

#include "../Graphics/SkeletalAnimationComponent.h"

#include "../Scene/SceneManager.h"

#include "../Scripting/ScriptingManager.h"

#include "../Utils/ResourceManager.h"


void RObjectLoader::loadComponents(XMLElement* objectElement, RObject* object)
{
	XMLElement* componentsElement = objectElement->FirstChildElement("Components");
	if (componentsElement == nullptr)
	{
		LOG_ERROR("Components element not found!");
		return;
	}

	XMLElement* componentElement = componentsElement->FirstChildElement("Component");
	while (componentElement != nullptr)
	{
		std::string componentType = componentElement->Attribute("type");
		LOG_INFO("Component: " + componentType);

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
		else if (componentType == "crossroad")
		{
			loadCrossroadComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "skeletalAnimation")
		{
			loadSkeletalAnimation(componentElement, object, componentIndex);
		}
		else if (componentType == "vehicle")
		{
			loadVehicle(componentElement, object, componentIndex);
		}
		else if (componentType == "aiAgent")
		{
			loadAiAgent(componentElement, object, componentIndex);
		}
		else if (componentType == "aiAgentVehicle")
		{
			loadAiAgentVehicle(componentElement, object, componentIndex);
		}
		else if (componentType == "trafficLights")
		{
			loadTrafficLightsComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "script")
		{
			loadScriptComponent(componentElement, object, componentIndex);
		}

		componentElement = componentElement->NextSiblingElement("Component");
	}

}


void RObjectLoader::loadRenderComponent(XMLElement* componentElement, RObject* object, int componentIndex)
{
	object->getComponents()[componentIndex]["model"] = componentElement->Attribute("model");
	object->getComponents()[componentIndex]["lowPolyModel"] = XmlUtils::getAttributeStringOptional(componentElement, "lowPolyModel");

	object->getComponents()[componentIndex]["dynamic"] = XmlUtils::getAttributeStringOptional(componentElement, "dynamic");
	object->getComponents()[componentIndex]["castShadows"] = XmlUtils::getAttributeStringOptional(componentElement, "castShadows", "true");
	object->getComponents()[componentIndex]["normalsSmoothing"] = XmlUtils::getAttributeStringOptional(componentElement, "normalsSmoothing", "true");
	object->getComponents()[componentIndex]["loadWithHierarchy"] = XmlUtils::getAttributeStringOptional(componentElement, "loadWithHierarchy", "false");
	object->getComponents()[componentIndex]["lowPolyModelNormalsSmoothing"] = XmlUtils::getAttributeStringOptional(componentElement, "lowPolyModelNormalsSmoothing", "true");
	object->getComponents()[componentIndex]["lowPolyLoadWithHierarchy"] = XmlUtils::getAttributeStringOptional(componentElement, "lowPolyLoadWithHierarchy", "false");
	object->getComponents()[componentIndex]["animated"] = XmlUtils::getAttributeStringOptional(componentElement, "animated");
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
		object->getComponents()[componentIndex]["centerOfMassOffset"] = XmlUtils::getAttributeStringOptional(componentElement, "centerOfMassOffset", "false");
		object->getComponents()[componentIndex]["centerOfMassOffsetValue"] = XmlUtils::getAttributeStringOptional(componentElement, "centerOfMassOffsetValue", "0,0,0");
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
	LOG_INFO("Sound play distance: " + playDistance);
	object->getComponents()[componentIndex]["playDistance"] = playDistance;

	// if set in config file - we set volume accordingly (otherwise - volume is 1.0f by default)
	std::string volume = XmlUtils::getAttributeStringOptional(componentElement, "volume", "1.0");
	LOG_INFO("Sound volume: " + volume);
	object->getComponents()[componentIndex]["volume"] = volume;

	std::string position = XmlUtils::getAttributeStringOptional(componentElement, "position", "0,0,0");
	LOG_INFO("Sound position offset: " + position);
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


void RObjectLoader::loadCrossroadComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	int index = 0;

	XMLElement* connectionPointElement = componentElement->FirstChildElement("ConnectionPoint");
	while (connectionPointElement != nullptr)
	{
		object->getComponents()[componentIndex]["position#" + toString(index)] = connectionPointElement->Attribute("position");
		object->getComponents()[componentIndex]["direction#" + toString(index)] = connectionPointElement->Attribute("direction");

		++index;
		connectionPointElement = connectionPointElement->NextSiblingElement("ConnectionPoint");
	}
	object->getComponents()[componentIndex]["pointsCount"] = toString(index);
}


void RObjectLoader::loadSkeletalAnimation(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	object->getComponents()[componentIndex]["animation"] = componentElement->Attribute("animation");
	object->getComponents()[componentIndex]["animation2"] = XmlUtils::getAttributeStringOptional(componentElement, "animation2");
	object->getComponents()[componentIndex]["rootBone"] = XmlUtils::getAttributeStringOptional(componentElement, "rootBone");
	object->getComponents()[componentIndex]["lockRootBoneTranslation"] = XmlUtils::getAttributeStringOptional(componentElement, "lockRootBoneTranslation", "true");
	object->getComponents()[componentIndex]["scale"] = XmlUtils::getAttributeStringOptional(componentElement, "scale", "1");
}


void RObjectLoader::loadVehicle(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	int index = 0;

	object->getComponents()[componentIndex]["mass"] = XmlUtils::getAttributeStringOptional(componentElement, "mass");

	XMLElement* wheelPointElement = componentElement->FirstChildElement("Wheel");
	while (wheelPointElement != nullptr)
	{
		object->getComponents()[componentIndex]["wheel_name#" + toString(index)] = wheelPointElement->Attribute("name");
		object->getComponents()[componentIndex]["wheel_side#" + toString(index)] = wheelPointElement->Attribute("side");
		object->getComponents()[componentIndex]["wheel_steering#" + toString(index)] = wheelPointElement->Attribute("steering");
		object->getComponents()[componentIndex]["wheel_powered#" + toString(index)] = wheelPointElement->Attribute("powered");
		object->getComponents()[componentIndex]["wheel_handbrake#" + toString(index)] = wheelPointElement->Attribute("handbrake");
		object->getComponents()[componentIndex]["wheel_model#" + toString(index)] = wheelPointElement->Attribute("model");
		object->getComponents()[componentIndex]["wheel_position#" + toString(index)] = wheelPointElement->Attribute("position");
		object->getComponents()[componentIndex]["wheel_radius#" + toString(index)] = wheelPointElement->Attribute("radius");
		object->getComponents()[componentIndex]["wheel_suspensionRestLength#" + toString(index)] = wheelPointElement->Attribute("suspensionRestLength");
		object->getComponents()[componentIndex]["wheel_suspensionStiffness#" + toString(index)] = wheelPointElement->Attribute("suspensionStiffness");
		object->getComponents()[componentIndex]["wheel_dampingCompression#" + toString(index)] = wheelPointElement->Attribute("dampingCompression");
		object->getComponents()[componentIndex]["wheel_dampingRelaxation#" + toString(index)] = wheelPointElement->Attribute("dampingRelaxation");
		object->getComponents()[componentIndex]["wheel_frictionSlip#" + toString(index)] = wheelPointElement->Attribute("frictionSlip");
		object->getComponents()[componentIndex]["wheel_rollInfluence#" + toString(index)] = wheelPointElement->Attribute("rollInfluence");
		object->getComponents()[componentIndex]["wheel_brakeForce#" + toString(index)] = wheelPointElement->Attribute("brakeForce");

		++index;
		wheelPointElement = wheelPointElement->NextSiblingElement("Wheel");
	}

	object->getComponents()[componentIndex]["wheelsCount"] = toString(index);
}


void RObjectLoader::loadAiAgent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	object->getComponents()[componentIndex]["speed"] = componentElement->Attribute("speed");
}


void RObjectLoader::loadAiAgentVehicle(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	object->getComponents()[componentIndex]["frontSensorPosition"] = componentElement->Attribute("frontSensorPosition");
}


void RObjectLoader::loadTrafficLightsComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	object->getComponents()[componentIndex]["redLightNodeName"] = componentElement->Attribute("redLightNodeName");
	object->getComponents()[componentIndex]["yellowLightNodeName"] = componentElement->Attribute("yellowLightNodeName");
	object->getComponents()[componentIndex]["greenLightNodeName"] = componentElement->Attribute("greenLightNodeName");
	object->getComponents()[componentIndex]["triggerBoxPosition"] = componentElement->Attribute("triggerBoxPosition");
	object->getComponents()[componentIndex]["triggerBoxRotation"] = componentElement->Attribute("triggerBoxRotation");
	object->getComponents()[componentIndex]["triggerBoxSize"] = componentElement->Attribute("triggerBoxSize");
	object->getComponents()[componentIndex]["stopPointPosition"] = componentElement->Attribute("stopPointPosition");
	object->getComponents()[componentIndex]["initState"] = componentElement->Attribute("initState");
}


void RObjectLoader::loadScriptComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex)
{
	object->getComponents()[componentIndex]["fileName"] = componentElement->Attribute("fileName");
}


// flag normalSmoothing and loadWithHierarchy only for non animated objects
RStaticModel* RObjectLoader::loadModel(const std::string& modelPath, const std::string& objectDirPath, bool isAnimated, bool normalSmoothing, bool loadWithHierarchy, RStaticModel* hightPollyModel/* = nullptr*/)
{
	if (isAnimated)
	{
		return ResourceManager::getInstance().loadAnimatedModel(modelPath, objectDirPath,
																hightPollyModel != nullptr ? static_cast<RAnimatedModel*>(hightPollyModel)->getBoneInfos() : std::unordered_map<std::string, BoneInfo*>());
	}
	else if (loadWithHierarchy)
	{
		return ResourceManager::getInstance().loadModelWithHierarchy(modelPath, objectDirPath, normalSmoothing);
	}
	else
	{
		return ResourceManager::getInstance().loadModel(modelPath, objectDirPath, normalSmoothing);
	}
}


RObject* RObjectLoader::loadObject(const std::string& dirPath, const std::string& originalName)
{
	const std::string& fullPath = dirPath + OBJECT_FILE_NAME;

	XMLDocument doc;
	XMLError result = doc.LoadFile(fullPath.c_str());
	if (result != XML_SUCCESS)
	{
		LOG_ERROR("Cannot read xml file: " + fullPath + "! Result: " + Strings::toString((int)result));
	}
	
	// Search for main element - Object
	XMLElement* objElement = doc.FirstChildElement("Object");
	if (objElement == nullptr)
	{
		LOG_ERROR("Object element not found!");
		return nullptr;
	}

	XMLElement* objDesc = objElement->FirstChildElement("Description");
	if (objDesc == nullptr)
	{
		LOG_ERROR("Description element not found!");
		return nullptr;
	}

	// Load file description
	std::string author(objDesc->Attribute("author"));
	std::string objectName(objDesc->Attribute("name"));
	std::string comment(objDesc->Attribute("comment"));

	LOG_INFO("*** OBJECT DATA ***");
	LOG_INFO("Author: " + author);
	LOG_INFO("Name: " + objectName);
	LOG_INFO("Comment: " + comment);


	RObject* object = new RObject(dirPath, author, objectName, comment, originalName);


	loadComponents(objElement, object);


	return object;
}


SceneObject* RObjectLoader::createSceneObjectFromRObject(RObject* objectDefinition, const std::string& name,
														 const glm::vec3& position, const glm::vec3& rotation, SceneManager* sceneManager)
{
	std::string objectDirPath = objectDefinition->getPath();

	SceneObject* sceneObject = sceneManager->addSceneObject(name, 0, objectDefinition);
	sceneObject->setPosition(position);
	sceneObject->setRotation(degToRad(rotation.x), degToRad(rotation.y), degToRad(rotation.z));

	RStaticModel* model = nullptr;

	std::vector<std::unordered_map<std::string, std::string>>& components = objectDefinition->getComponents();

	for (int i = 0; i < components.size(); ++i)
	{
		std::string componentType = components[i]["type"];

		if (componentType == "render")
		{
			GraphicsManager* graphicsManager = sceneManager->getGraphicsManager();

			const std::string& modelFile = components[i]["model"];
			const std::string& modelPath = objectDirPath + modelFile;
			bool isAnimated = toBool(components[i]["animated"]);
			bool normalsSmoothing = toBool(components[i]["normalsSmoothing"]);
			bool loadWithHierarchy = toBool(components[i]["loadWithHierarchy"]);

			model = loadModel(modelPath, objectDirPath, isAnimated, normalsSmoothing, loadWithHierarchy);

			RenderObject* renderObject = graphicsManager->addRenderObject(new RenderObject(model), sceneObject);
			renderObject->setDynamicObject(toBool(components[i]["dynamic"]));
			renderObject->setCastShadows(toBool(components[i]["castShadows"]));

			const std::string& lowPolyModeFile = components[i]["lowPolyModel"];
			if (!lowPolyModeFile.empty())
			{
				const std::string lowPolyModelPath = objectDirPath + lowPolyModeFile;
				bool lowPolyNormalsSmoothing = toBool(components[i]["lowPolyModelNormalsSmoothing"]);
				bool lowPolyLoadWithHierarchy = toBool(components[i]["lowPolyLoadWithHierarchy"]);

				RStaticModel* lowPolyModel = loadModel(lowPolyModelPath, objectDirPath, isAnimated, lowPolyNormalsSmoothing, lowPolyLoadWithHierarchy, model);
				renderObject->setModel(lowPolyModel, 1);
			}
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
				LOG_INFO("- Creating dynamic Convex Hull collision shape");

				float mass = toFloat(components[i]["mass"]);
				bool centerOfMassOffset = toBool(components[i]["centerOfMassOffset"]);
				btVector3 centerOfMassOffsetValue = XMLstringToBtVec3(components[i]["centerOfMassOffsetValue"].c_str());
				int collidesWith = COL_TERRAIN | COL_WHEEL | COL_BUS | COL_DOOR | COL_ENV;

				PhysicalBodyConvexHull* physicalBody = physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), mass,
					COL_ENV, collidesWith, centerOfMassOffset, centerOfMassOffsetValue);

				sceneObject->addComponent(physicalBody);
			}
			else if (bodyType == "static")
			{
				LOG_INFO("- Creating static Convex Hull collision shape");

				int collidesWith = COL_WHEEL | COL_BUS | COL_DOOR | COL_ENV;

				PhysicalBodyConvexHull* physicalBody = physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), 0,
					COL_ENV, collidesWith);
				//terrainMesh->setRestitution(0.9f);
				//terrainMesh->getRigidBody()->setFriction(1.0f);
				sceneObject->addComponent(physicalBody);
			}
			else if (bodyType == "bvh")
			{
				LOG_INFO("- Creating BVH Triangle Mesh collision shape");

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
			BusStopComponent* component = sceneManager->getBusStopSystem()->addBusStopComponent("Przystanek");
			sceneObject->addComponent(component);
		}
		else if (componentType == "environmentCapture")
		{
			GraphicsManager* graphicsManager = sceneManager->getGraphicsManager();

			std::string textures = components[i]["textures"];
			std::vector<std::string> t = split(textures, ',');

			RTextureCubeMap* cubeMap = ResourceManager::getInstance().loadTextureCubeMap(&t[0]);
			EnvironmentCaptureComponent* component = graphicsManager->addEnvironmentCaptureComponent(cubeMap);
			sceneObject->addComponent(component);
		}
		else if (componentType == "crossroad")
		{
			GraphicsManager* graphicsManager = sceneManager->getGraphicsManager();

			int pointsCount = toInt(components[i]["pointsCount"]);
			std::vector<CrossroadConnectionPoint> connectionPoints(pointsCount);

			for (int j = 0; j < pointsCount; ++j)
			{
				connectionPoints[j].position = XMLstringToVec3(components[i]["position#" + toString(j)].c_str());
				connectionPoints[j].direction = XMLstringToVec3(components[i]["direction#" + toString(j)].c_str());
			}

			CrossroadComponent* crossroad = graphicsManager->addCrossRoad(connectionPoints);
			sceneObject->addComponent(crossroad);
		}
		else if (componentType == "skeletalAnimation")
		{
			GraphicsManager* graphicsManager = sceneManager->getGraphicsManager();

			const std::string& animationFile = components[i]["animation"];
			const std::string& animationFile2 = components[i]["animation2"];

			const std::string& rootBone = components[i]["rootBone"];
			bool lockRootBoneTranslation = toBool(components[i]["lockRootBoneTranslation"]);
			float scale = toFloat(components[i]["scale"]);

			RAnimation* animation = ResourceManager::getInstance().loadAnimation(GameDirectories::ANIMATIONS + animationFile);
			RAnimation* animation2 = animationFile2 != "" ? ResourceManager::getInstance().loadAnimation(GameDirectories::ANIMATIONS + animationFile2) : nullptr;
			SkeletalAnimationComponent* skeletalAnimation = graphicsManager->addSkeletalAnimation(animation, animation2);
			sceneObject->addComponent(skeletalAnimation);
			sceneObject->setScale(scale);

			if (!rootBone.empty())
			{
				skeletalAnimation->setRootBone(rootBone);
			}
			skeletalAnimation->setLockRootBoneTranslation(lockRootBoneTranslation);
			skeletalAnimation->setScale(scale);
		}
		else if (componentType == "vehicle")
		{
			PhysicsManager* physicsManager = sceneManager->getPhysicsManager();
			GraphicsManager* graphicsManager = sceneManager->getGraphicsManager();

			float mass = toFloat(components[i]["mass"]);

			int collidesWith = COL_TERRAIN | COL_ENV | COL_BUS;
			int wheelCollidesWith = COL_TERRAIN | COL_ENV;

			PhysicalBodyRaycastVehicle* vehicle = physicsManager->createPhysicalBodyRayCastVehicle(model->getCollisionMesh(), model->getCollisionMeshSize(), mass, COL_BUS, collidesWith);

			vehicle->setWheelCollisionFilter(COL_WHEEL, wheelCollidesWith);
			vehicle->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);

			sceneObject->addComponent(vehicle);

			// wheels
			int wheelsCount = toInt(components[i]["wheelsCount"]);
			for (int j = 0; j < wheelsCount; ++j)
			{
				const std::string& wheelName = components[i]["wheel_name#" + toString(j)];
				const std::string& wheelModel = components[i]["wheel_model#" + toString(j)];
				const std::string& side = components[i]["wheel_side#" + toString(j)];
				float radius = toFloat(components[i]["wheel_radius#" + toString(j)]);

				int steering = toInt(components[i]["wheel_steering#" + toString(j)]);
				int powered = toInt(components[i]["wheel_powered#" + toString(j)]);
				int handbrake = toInt(components[i]["wheel_handbrake#" + toString(j)]);

				float suspensionRestLength = toFloat(components[i]["wheel_suspensionRestLength#" + toString(j)]);
				float suspensionStiffness = toFloat(components[i]["wheel_suspensionStiffness#" + toString(j)]);
				float dampingCompression = toFloat(components[i]["wheel_dampingCompression#" + toString(j)]);
				float dampingRelaxation = toFloat(components[i]["wheel_dampingRelaxation#" + toString(j)]);
				float frictionSlip = toFloat(components[i]["wheel_frictionSlip#" + toString(j)]);
				float rollInfluence = toFloat(components[i]["wheel_rollInfluence#" + toString(j)]);
				float brakeForce = toFloat(components[i]["wheel_brakeForce#" + toString(j)]);

				glm::vec3 wheelPosition = XMLstringToVec3(components[i]["wheel_position#" + toString(j)].c_str());

				SceneObject* wheelObj = sceneManager->addSceneObject(wheelName);
				wheelObj->setFlags(SOF_NOT_SELECTABLE | SOF_NOT_SERIALIZABLE);

				// obracamy model kola je¿li jest po lewej stronie
				// tak naprawde powinnismy obracac kolo po prawej stronie, ale przez AXLE ustawione na -1, 0, 0 bullet obraca nam kola o 180 stopni. Dlatego my obracamy lewe

				float wheelAngle;
				if (GameConfig::getInstance().mode == GM_GAME)
				{
					wheelAngle = side == "right" ? 0.0f : 180.0f;
				}
				else
				{
					wheelAngle = side == "right" ? 180.0f : 0.0f;
				}


				SceneObject* wheelSubObjectForModel = sceneManager->addSceneObject(wheelName + "Model");
				wheelSubObjectForModel->setFlags(SOF_NOT_SELECTABLE | SOF_NOT_SERIALIZABLE);
				wheelSubObjectForModel->setRotation(0.0f, degToRad(wheelAngle), 0.0f);
				wheelObj->addChild(wheelSubObjectForModel);

				std::string modelPath = objectDirPath + wheelModel;
				RStaticModel* wheel = ResourceManager::getInstance().loadModel(modelPath, objectDirPath, false);
				RenderObject* wheelRenderObject = graphicsManager->addRenderObject(new RenderObject(wheel), wheelSubObjectForModel);
				wheelRenderObject->setDynamicObject(true);


				btVector3 btWheelPos(wheelPosition.x, wheelPosition.y, wheelPosition.z);

				PhysicalBodyWheel* wheelBody = physicsManager->createPhysicalBodyWheel(vehicle, btWheelPos, suspensionRestLength, radius, steering);
				wheelObj->addComponent(wheelBody);

				btWheelInfo& wheelInfo = wheelBody->getWheelInfo();
				wheelInfo.m_suspensionStiffness = suspensionStiffness;
				wheelInfo.m_wheelsDampingCompression = dampingCompression * 2 * sqrt(wheelInfo.m_suspensionStiffness);
				wheelInfo.m_wheelsDampingRelaxation = dampingRelaxation * 2 * sqrt(wheelInfo.m_suspensionStiffness);
				wheelInfo.m_frictionSlip = frictionSlip;
				wheelInfo.m_rollInfluence = rollInfluence;

				if (GameConfig::getInstance().mode == GM_EDITOR)
				{
					wheelObj->setPosition(wheelPosition - glm::vec3(0.0f, suspensionRestLength, 0.0f));
					sceneObject->addChild(wheelObj);
				}
			}
		}
		else if (componentType == "aiAgent")
		{
			float speed = toFloat(components[i]["speed"]);

			AIAgent* aiAgent = sceneManager->getGameLogicSystem()->addAIAgent();
			aiAgent->setSpeed(speed);

			sceneObject->addComponent(aiAgent);
		}
		else if (componentType == "aiAgentVehicle")
		{
			glm::vec3 frontSensorPosition = XMLstringToVec3(components[i]["frontSensorPosition"].c_str());

			AIAgentVehicle* aiAgent = sceneManager->getGameLogicSystem()->addAIAgentVehicle();
			aiAgent->setFrontSensorPosition(frontSensorPosition);

			sceneObject->addComponent(aiAgent);
		}
		else if (componentType == "trafficLights")
		{
			const std::string& redLightNodeName = components[i]["redLightNodeName"];
			const std::string& yellowLightNodeName = components[i]["yellowLightNodeName"];
			const std::string& greenLightNodeName = components[i]["greenLightNodeName"];
			const std::string& triggerBoxPositionStr = components[i]["triggerBoxPosition"].c_str();
			const std::string& triggerBoxRotationStr = components[i]["triggerBoxRotation"].c_str();
			const std::string& triggerBoxSizeStr = components[i]["triggerBoxSize"].c_str();
			const std::string& stopPointPositionStr = components[i]["stopPointPosition"].c_str();
			const std::string& initStateStr = components[i]["initState"];

			glm::vec3 triggerBoxPosition = !triggerBoxPositionStr.empty() ? XMLstringToVec3(triggerBoxPositionStr.c_str()) : glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 triggerBoxRotation = !triggerBoxPositionStr.empty() ? XMLstringToVec3(triggerBoxRotationStr.c_str()) : glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 triggerBoxSize = !triggerBoxPositionStr.empty() ? XMLstringToVec3(triggerBoxSizeStr.c_str()) : glm::vec3(1.0f, 1.0f, 1.0f);
			glm::vec3 stopPointPosition = !stopPointPositionStr.empty() ? XMLstringToVec3(stopPointPositionStr.c_str()) : glm::vec3(0.0f, 0.0f, 0.0f);
			TrafficLightsState initState = !initStateStr.empty() ? getTrafficLightsStateFromString(initStateStr) : TLS_RED;

			TrafficLightsComponent* trafficLights = sceneManager->getGameLogicSystem()->addTrafficLightsComponent(redLightNodeName, yellowLightNodeName, greenLightNodeName, triggerBoxPosition, triggerBoxRotation, triggerBoxSize, stopPointPosition, initState);

			sceneObject->addComponent(trafficLights);
		}
		else if (componentType == "script")
		{
			const std::string& fileName = components[i]["fileName"];

			const std::string& scriptPath = objectDirPath + fileName;
			RScriptFile* scriptFile = ResourceManager::getInstance().loadScriptFile(scriptPath);

			ScriptComponent* scriptComponent = sceneManager->getScriptingManager()->addScript(scriptFile);

			sceneObject->addComponent(scriptComponent);
		}
	}

	return sceneObject;
}
