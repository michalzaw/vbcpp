#include "CrossroadComponent.h"

#include "RoadObject.h"

#include "../Scene/SceneObject.h"


CrossroadComponent::CrossroadComponent(std::vector<CrossroadConnectionPoint>& connectionPoints)
	: Component(CT_CROSSROAD),
	_connectionPoints(connectionPoints)
{

}


void CrossroadComponent::connectRoad(int connectionPointIndex, RoadObject* roadObject)
{
	if (connectionPointIndex >= _connectionPoints.size() && roadObject != nullptr)
		return;

	_connectionPoints[connectionPointIndex].connectedRoads.push_back(roadObject);
}


void CrossroadComponent::disconnectRoad(int connectionPointIndex, RoadObject* roadObject)
{
	if (connectionPointIndex >= _connectionPoints.size() && roadObject != nullptr)
		return;

	for (std::list<RoadObject*>::iterator i = _connectionPoints[connectionPointIndex].connectedRoads.begin();
		i != _connectionPoints[connectionPointIndex].connectedRoads.end();
		++i)
	{
		if ((*i) == roadObject)
		{
			i = _connectionPoints[connectionPointIndex].connectedRoads.erase(i);
			break;
		}
	}
}


int CrossroadComponent::getConnectionsCount()
{
	return _connectionPoints.size();
}


CrossroadConnectionPoint& CrossroadComponent::getConnectionPoint(int index)
{
	return _connectionPoints[index];
}


glm::vec3 CrossroadComponent::getGlobalPositionOfConnectionPoint(int index)
{
	return getSceneObject()->transformLocalPointToGlobal(_connectionPoints[index].position);
}


glm::vec3 CrossroadComponent::getGlobalDirectionOfConnectionPoint(int index)
{
	return getSceneObject()->transformLocalVectorToGlobal(_connectionPoints[index].direction);
}


void CrossroadComponent::changedTransform()
{
	for (CrossroadConnectionPoint& connectionPoint : _connectionPoints)
	{
		for (RoadObject* roadObject : connectionPoint.connectedRoads)
		{
			roadObject->resetConnectedPointPositions();
		}
	}
}
