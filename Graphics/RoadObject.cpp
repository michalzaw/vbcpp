#include "RoadObject.h"

#include "../Utils/Logger.h"


RoadObject::RoadObject(RRoadProfile* _roadProfile, std::vector<RoadSegment>& segments)
	: _roadProfile(_roadProfile), _segments(segments)
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
	/*
	// delete generated RStaticModel
	if (_model != nullptr)
	{
		delete _model;
	}*/

	RStaticModel* newModel;
	if (_model == nullptr)
	{
		newModel = createRoadModel(_roadProfile->getRoadLanes(), _roadProfile->getRoadLanes().size(), _segments);
	}
	else
	{
		if (_modelRootNode)
		{
			delete _modelRootNode;
		}

		if (_materials)
		{
			delete[] _materials;
		}

		newModel = createRoadModel(_roadProfile->getRoadLanes(), _roadProfile->getRoadLanes().size(), _segments, _model);
	}
	setModel(newModel);
}


RRoadProfile* RoadObject::getRoadProfile()
{
	return _roadProfile;
}


std::vector<RoadSegment>& RoadObject::getSegments()
{
	return _segments;
}

void RoadObject::setSegments(std::vector<RoadSegment> segments)
{
	_segments = segments;
}