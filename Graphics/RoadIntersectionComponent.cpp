#include "RoadIntersectionComponent.h"

#include "RoadObject.h"


RoadIntersectionComponent::RoadIntersectionComponent()
	: Component(CT_ROAD_INTERSECTION),
	_length(10.0f),
	_needRebuildConnectedRoad(false)
{

}


RoadIntersectionComponent::~RoadIntersectionComponent()
{

}


void RoadIntersectionComponent::connectRoad(RoadObject* roadObject, int connectionPointInRoadIndex)
{
	_roads.push_back(RoadConnectedToIntersection(roadObject, connectionPointInRoadIndex));
}


void RoadIntersectionComponent::disconnectRoad(RoadObject* roadObject, int connectionPointInRoadIndex)
{
	for (std::vector<RoadConnectedToIntersection>::iterator i = _roads.begin(); i != _roads.end(); ++i)
	{
		if (i->road == roadObject && i->connectionPointInRoadIndex == connectionPointInRoadIndex)
		{
			i = _roads.erase(i);
			break;
		}
	}
}


const std::vector<RoadConnectedToIntersection>& RoadIntersectionComponent::getConnectedRoads()
{
	return _roads;
}


void RoadIntersectionComponent::setLength(float length)
{
	_length = length;

	needRebuildConnectedRoad();
}


float RoadIntersectionComponent::getLength()
{
	return _length;
}


void RoadIntersectionComponent::needRebuildConnectedRoad()
{
	_needRebuildConnectedRoad = true;

	LOG_DEBUG("needRebuildConnectedRoad");
}


void RoadIntersectionComponent::changedTransform()
{
	for (const auto& connectedRoad : _roads)
	{
		connectedRoad.road->resetConnectedPointPositions();
	}
}


void RoadIntersectionComponent::update(float deltaTime)
{
	static float roadModificationTimer = 0.0f;
	roadModificationTimer += deltaTime;

	if (roadModificationTimer >= 0.2f && _needRebuildConnectedRoad)
	{
		LOG_DEBUG("BUILD ROAD: BEGIN");
		for (int i = 0; i < _roads.size(); ++i)
		{
			_roads[i].road->buildModel();
		}

		LOG_DEBUG("BUILD ROAD: END");
		roadModificationTimer = 0.0f;
		_needRebuildConnectedRoad = false;
	}
}
