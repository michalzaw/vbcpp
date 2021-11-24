#include "RRoadProfile.h"


RRoadProfile::RRoadProfile(std::string path, std::string author, std::string name, std::string comment)
	: Resource(RT_ROAD_PROFILE, path),
	_author(author), _name(name), _comment(comment)
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
