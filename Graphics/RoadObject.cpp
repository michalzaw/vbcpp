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
	std::vector<RoadConnectionPointData*> connectionPointsData;
	for (int i = 0; i < 2; ++i)
	{
		if (_connectionPoints[i].crossroadComponent != nullptr)
		{
			RoadConnectionPointData* data = new RoadConnectionPointData;

			glm::vec3 pos = _connectionPoints[i].crossroadComponent->getConnectionPoint(_connectionPoints[i].index).position;
			data->position = _connectionPoints[i].crossroadComponent->getSceneObject()->transformLocalPointToGlobal(pos);

			glm::vec3 dir = _connectionPoints[i].crossroadComponent->getConnectionPoint(_connectionPoints[i].index).direction;
			data->direction = _connectionPoints[i].crossroadComponent->getSceneObject()->transformLocalVectorToGlobal(dir);

			connectionPointsData.push_back(data);
		}
		else
		{
			connectionPointsData.push_back(nullptr);
		}
	}

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
		newModel = createRoadModel(_roadProfile->getRoadLanes(), _points, _segments, connectionPointsData);
	}
	else if (!reuseExistingModel)
	{
		if (_model != nullptr)
		{
			delete _model;
		}

		newModel = createRoadModel(_roadProfile->getRoadLanes(), _points, _segments, connectionPointsData);
	}
	else
	{
		newModel = createRoadModel(_roadProfile->getRoadLanes(), _points, _segments, connectionPointsData, _model);
	}
	setModel(newModel);

	for (int i = 0; i < 2; ++i)
	{
		if (connectionPointsData[i] != nullptr)
			delete connectionPointsData[i];
	}
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

	if (_connectionPoints[index].crossroadComponent != nullptr)
	{
		_connectionPoints[index].crossroadComponent->disconnectRoad(_connectionPoints[index].index, this);
	}

	if (crossroadComponent != nullptr)
	{
		crossroadComponent->connectRoad(indexInCrossroad, this);
	}

	_connectionPoints[index].crossroadComponent = crossroadComponent;
	_connectionPoints[index].index = indexInCrossroad;
}


RoadConnectionPoint& RoadObject::getConnectionPoint(int index)
{
	return _connectionPoints[index];
}
