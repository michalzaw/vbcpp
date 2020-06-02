#include "RoadObject.h"

#include <algorithm>

#include "CrossroadComponent.h"

#include "../Scene/SceneObject.h"

#include "../Utils/Logger.h"


RoadObject::RoadObject(RRoadProfile* _roadProfile, std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments)
	: _roadProfile(_roadProfile), _points(points), _segments(segments), _connectionPoints(2)
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
		newModel = createRoadModel(_roadProfile->getRoadLanes(), _points, _segments);
	}
	else if (!reuseExistingModel)
	{
		if (_model != nullptr)
		{
			delete _model;
		}

		newModel = createRoadModel(_roadProfile->getRoadLanes(), _points, _segments);
	}
	else
	{
		newModel = createRoadModel(_roadProfile->getRoadLanes(), _points, _segments, _model);
	}
	setModel(newModel);
}


RRoadProfile* RoadObject::getRoadProfile()
{
	return _roadProfile;
}


std::vector<glm::vec3>& RoadObject::getPoints()
{
	return _points;
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
	_points.push_back(position);

	if (_points.size() > 1)
	{
		_segments.push_back(RoadSegment());
	}
}


void RoadObject::deletePoint(unsigned int index)
{
	if (index >= _points.size())
	{
		return;
	}

	_points.erase(_points.begin() + index);

	unsigned int segmentIndex = std::max((int)index - 1, 0);

	if (segmentIndex < _segments.size())
	{
		_segments.erase(_segments.begin() + segmentIndex);
	}
}


void RoadObject::setConnectionPoint(int index, CrossroadComponent* crossroadComponent, int indexInCrossroad)
{
	if (index >= 2)
		return;

	if (crossroadComponent != nullptr)
	{
		crossroadComponent->connectRoad(indexInCrossroad, this);
	}
	else if (_connectionPoints[index].crossroadComponent != nullptr)
	{
		_connectionPoints[index].crossroadComponent->connectRoad(indexInCrossroad, nullptr);
	}

	_connectionPoints[index].crossroadComponent = crossroadComponent;
	_connectionPoints[index].index = indexInCrossroad;
}


RoadConnectionPoint& RoadObject::getConnectionPoint(int index)
{
	return _connectionPoints[index];
}
