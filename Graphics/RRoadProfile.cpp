#include "RRoadProfile.h"


RoadProfileType getRoadProfileTypeFromString(const std::string& name)
{
	for (int i = 0; i < RPT_COUNT; ++i)
	{
		if (roadProfileTypeStrings[i] == name)
			return static_cast<RoadProfileType>(i);
	}
}


AIPathIntersectionMode getAIPathIntersectionModeFromString(const std::string& name)
{
	for (int i = 0; i < APIM_COUNT; ++i)
	{
		if (aIPathIntersectionModeStrings[i] == name)
			return static_cast<AIPathIntersectionMode>(i);
	}
}


RRoadProfile::RRoadProfile(std::string path, std::string author, std::string name, std::string comment, Material* intersectionMaterial, float intersectionRoadY, RoadProfileType type)
	: Resource(RT_ROAD_PROFILE, path),
	_type(type),
	_author(author), _name(name), _comment(comment),
	_intersectionMaterial(intersectionMaterial), _intersectionRoadY(intersectionRoadY),
	_maxX(0.0f), _minX(0.0f), _maxXIsCalculated(false), _minXIsCalculated(false)
{

}


RRoadProfile::~RRoadProfile()
{
	
}


RoadProfileType RRoadProfile::getType()
{
	return _type;
}


std::string RRoadProfile::getAuthor()
{
	return _author;
}


std::string RRoadProfile::getName()
{
	return _name;
}


std::string RRoadProfile::getComment()
{
	return _comment;
}


std::vector<RoadLane>& RRoadProfile::getRoadLanes()
{
	return _roadLanes;
}


std::vector<glm::vec2>& RRoadProfile::getEdges()
{
	return _edges;
}


Material* RRoadProfile::getIntersectionMaterial()
{
	return _intersectionMaterial;
}


float RRoadProfile::getIntersectionRoadY()
{
	return _intersectionRoadY;
}


std::vector<AIPath>& RRoadProfile::getAIPaths()
{
	return _aiPaths;
}


float RRoadProfile::getMaxX()
{
	if (!_maxXIsCalculated)
	{
		for (const auto& lane : _roadLanes)
		{
			_maxX = std::max(_maxX, lane.r1);
			_maxX = std::max(_maxX, lane.r2);
		}

		_maxXIsCalculated = true;
	}

	return _maxX;
}


float RRoadProfile::getMinX()
{
	if (!_minXIsCalculated)
	{
		for (const auto& lane : _roadLanes)
		{
			_minX = std::min(_minX, lane.r1);
			_minX = std::min(_minX, lane.r2);
		}

		_minXIsCalculated = true;
	}

	return _minX;
}
