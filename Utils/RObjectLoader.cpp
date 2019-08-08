#include "RObjectLoader.h"

#include <sstream>
#include <cstdlib>

#include "Logger.h"
#include "Helpers.hpp"
#include "Strings.h"
#include "XmlUtils.h"

#include "tinyxml2.h"
using namespace tinyxml2;


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


RObject* RObjectLoader::loadObject(std::string fullPath)
{
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


	RObject* object = new RObject(fullPath, author, objectName, comment);


	loadComponents(objElement, object);


	return object;
}
