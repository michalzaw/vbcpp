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


void RoadObject::buildModel(bool reuseExistingModel)
{
	// delete objects in base class RenderObject
	/*
	// delete generated RStaticModel
	if (_model != nullptr)
	{
		delete _model;
	}*/

	if (_modelRootNode)
	{
		delete _modelRootNode;
	}

	if (_materials)
	{
		delete[] _materials;
	}

	RStaticModel* newModel;
	if (_model == nullptr)
	{
		newModel = createRoadModel(_roadProfile->getRoadLanes(), _segments);
	}
	else if (!reuseExistingModel)
	{
		if (_model != nullptr)
		{
			delete _model;
		}

		newModel = createRoadModel(_roadProfile->getRoadLanes(), _segments);
	}
	else
	{
		newModel = createRoadModel(_roadProfile->getRoadLanes(), _segments, _model);
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


void RoadObject::setRoadProfile(RRoadProfile* roadProfile)
{
	_roadProfile = roadProfile;
}


void RoadObject::setSegments(std::vector<RoadSegment> segments)
{
	_segments = segments;
}


void RoadObject::addPoint(glm::vec3 position)
{

}


void RoadObject::deletePoint(unsigned int index)
{
	if (index > _segments.size())
	{
		return;
	}

	if (index == 0)
	{
		_segments.erase(_segments.begin());
	}
	else if (index == _segments.size())
	{
		_segments.erase(_segments.end() - 1);
	}
	else
	{
		_segments[index - 1].end = _segments[index].end;
		_segments.erase(_segments.begin() + index);
	}
}
