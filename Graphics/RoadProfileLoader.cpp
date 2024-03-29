#include "RoadProfileLoader.h"

#include "LoadMaterial.h"

#include "../Utils/Logger.h"

#include "../Utils/tinyxml2.h"
using namespace tinyxml2;


RRoadProfile* RoadProfileLoader::loadRoadProfile(std::string dirPath)
{
	std::string fullPath = dirPath + PROFILE_FILE_NAME;
	std::string materialFullPath = MaterialLoader::createMaterialFileName(fullPath);

	XMLDocument doc;
	XMLError result = doc.LoadFile(fullPath.c_str());
	if (result != XML_SUCCESS)
	{
		LOG_ERROR("Cannot read xml file: " + fullPath + "! Result: " + Strings::toString((int)result));
	}

	// Search for main element - Profile
	XMLElement* profileElement = doc.FirstChildElement("Profile");
	if (profileElement == nullptr)
	{
		LOG_ERROR("Profile element not found!");
		return nullptr;
	}

	XMLElement* profileDesc = profileElement->FirstChildElement("Description");
	if (profileDesc == nullptr)
	{
		LOG_ERROR("Description element not found!");
		return nullptr;
	}

	// Load file description
	std::string author(profileDesc->Attribute("author"));
	std::string profName(profileDesc->Attribute("name"));
	std::string comment(profileDesc->Attribute("comment"));

	LOG_INFO("*** PROFILE DATA ***");
	LOG_INFO("Author: " + author);
	LOG_INFO("Name: " + profName);
	LOG_INFO("Comment: " + comment);


	MaterialLoader matLoader;
	matLoader.openFile(materialFullPath.c_str());


	XMLElement* intersectionDataElement = profileElement->FirstChildElement("IntersectionData");

	Material* intersectionMaterial = nullptr;
	float intersectionRoadY = 0.0f;
	if (intersectionDataElement != nullptr)
	{
		intersectionMaterial = matLoader.loadMaterial(intersectionDataElement->Attribute("material"), dirPath);
		intersectionRoadY = toFloat(intersectionDataElement->Attribute("y"));
	}


	RRoadProfile* roadProfile = new RRoadProfile(dirPath, author, profName, comment, intersectionMaterial, intersectionRoadY);


	XMLElement* lanesElement = profileElement->FirstChildElement("Lanes");

	if (lanesElement == nullptr)
	{
		LOG_ERROR("Lanes element not found");
		delete roadProfile;
		return nullptr;
	}
	else
	{
		XMLElement* laneElement = lanesElement->FirstChildElement("Lane");
		while (laneElement != nullptr)
		{
			RoadLane lane;
			lane.material = matLoader.loadMaterial(laneElement->Attribute("material"), dirPath);
			lane.r1 = toFloat(laneElement->Attribute("x1"));
			lane.r2 = toFloat(laneElement->Attribute("x2"));
			lane.height1 = toFloat(laneElement->Attribute("y1"));
			lane.height2 = toFloat(laneElement->Attribute("y2"));

			roadProfile->getRoadLanes().push_back(lane);

			laneElement = laneElement->NextSiblingElement("Lane");
		}
	}


	XMLElement* edgesElement = profileElement->FirstChildElement("Edges");

	if (edgesElement != nullptr)
	{
		XMLElement* edgeElement = edgesElement->FirstChildElement("Edge");
		while (edgeElement != nullptr)
		{
			float x = toFloat(edgeElement->Attribute("x"));
			float y = toFloat(edgeElement->Attribute("y"));

			roadProfile->getEdges().push_back(glm::vec2(x, y));

			edgeElement = edgeElement->NextSiblingElement("Edge");
		}
	}

	XMLElement* pathsElement = profileElement->FirstChildElement("AIPaths");
	if (pathsElement != nullptr)
	{
		XMLElement* pathElement = pathsElement->FirstChildElement("Path");
		while (pathElement != nullptr)
		{
			AIPath aiPath;
			aiPath.name = pathElement->Attribute("name");
			aiPath.x = toFloat(pathElement->Attribute("x"));
			aiPath.y = toFloat(pathElement->Attribute("y"));
			aiPath.direction = toInt(pathElement->Attribute("direction"));

			roadProfile->getAIPaths().push_back(aiPath);

			pathElement = pathElement->NextSiblingElement("Path");
		}
	}


	return roadProfile;
}
