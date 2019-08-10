#include "RoadObject.h"

#include "../Utils/Logger.h"


RoadObject::RoadObject(std::vector<RoadLane>& roadLanes, std::vector<RoadSegment>& segments)
	: _roadLanes(roadLanes), _segments(segments)
{
	_type = CT_ROAD_OBJECT;

	buildModel();
}


RoadObject::~RoadObject()
{
	Logger::info("RoadObject destructor\n");
	if (_model != nullptr)
	{
		delete _model;
	}
}


void RoadObject::buildModel()
{
	// delete objects in base class RenderObject
	if (_modelRootNode)
	{
		delete _modelRootNode;
	}

	if (_materials)
	{
		delete[] _materials;
	}

	// delete generated RStaticModel
	if (_model != nullptr)
	{
		delete _model;
	}

	setModel(createRoadModel(_roadLanes, _roadLanes.size(), _segments));
}
