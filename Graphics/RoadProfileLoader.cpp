#include "RoadProfileLoader.h"

#include "LoadMaterial.h"

#include "../Utils/Logger.h"


RRoadProfile* RoadProfileLoader::loadRoadProfile(std::string dirPath)
{
	std::string fullPath = dirPath + PROFILE_FILE_NAME;
	std::string materialFullPath = MaterialLoader::createMaterialFileName(fullPath);

	XMLDocument doc;
	XMLError result = doc.LoadFile(fullPath.c_str());
	if (result != XML_SUCCESS)
	{
		Logger::error("Cannot read xml file: " + fullPath + "! Result: " + toString(result));
	}

	// Search for main element - Profile
	XMLElement* profileElement = doc.FirstChildElement("Profile");
	if (profileElement == nullptr)
	{
		Logger::error("Profile element not found!");
		return nullptr;
	}

	XMLElement* profileDesc = profileElement->FirstChildElement("Description");
	if (profileDesc == nullptr)
	{
		Logger::error("Description element not found!");
		return nullptr;
	}

	// Load file description
	std::string author(profileDesc->Attribute("author"));
	std::string profName(profileDesc->Attribute("name"));
	std::string comment(profileDesc->Attribute("comment"));

	Logger::info("*** PROFILE DATA ***");
	Logger::info("Author: " + author);
	Logger::info("Name: " + profName);
	Logger::info("Comment: " + comment);


	RRoadProfile* roadProfile = new RRoadProfile(dirPath, author, profName, comment);


	MaterialLoader matLoader;
	matLoader.openFile(materialFullPath.c_str());

	XMLElement* lanesElement = profileElement->FirstChildElement("Lanes");

	if (lanesElement == nullptr)
	{
		Logger::error("Lanes element not found");
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

	return roadProfile;
}
