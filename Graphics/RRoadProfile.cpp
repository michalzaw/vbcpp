#include "RRoadProfile.h"


RRoadProfile::RRoadProfile(std::string path, std::string author, std::string name, std::string comment)
	: Resource(RT_ROAD_PROFILE, path),
	_author(author), _name(name), _comment(comment),
	_maxX(0.0f), _minX(0.0f), _maxXIsCalculated(false), _minXIsCalculated(false)
{

}


RRoadProfile::~RRoadProfile()
{
	for (RoadLane& roadLane : _roadLanes)
	{
		if (roadLane.material != nullptr)
		{
			delete roadLane.material;
		}
	}
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
