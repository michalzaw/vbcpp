#include "RoadIntersectionComponent.h"

#include "ShapePolygonComponent.h"
#include "RoadGenerator.h"
#include "RoadObject.h"

#include "../Scene/SceneObject.h"


RoadIntersectionComponent::RoadIntersectionComponent()
	: Component(CT_ROAD_INTERSECTION),
	_length(10.0f), _width(5.0f),
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


void RoadIntersectionComponent::setWidth(float width)
{
	_width = width;

	needRebuildConnectedRoad();
}


float RoadIntersectionComponent::getLength()
{
	return _length;
}


float RoadIntersectionComponent::getWidth()
{
	return _width;
}


void RoadIntersectionComponent::createPolygon()
{
	auto shapePolygonComponent = new ShapePolygonComponent;
	getSceneObject()->removeComponent(getSceneObject()->getComponent(CT_SHAPE_POLYGON));
	getSceneObject()->addComponent(shapePolygonComponent);

	std::vector<RoadConnectionPointData*> temp(2, nullptr);

	glm::vec3 centerPoint = getSceneObject()->getGlobalPosition();

	for (int i = 0; i < _roads.size(); ++i)
	//for (int i = 0; i < 1; ++i)
	{
		int road1Index = i;
		int road2Index = (i + 1) % _roads.size();

		const auto& road1CurvePoints = _roads[road1Index].road->getCurvePoints();
		const auto& road2CurvePoints = _roads[road2Index].road->getCurvePoints();
		int road1CenterPointIndex = _roads[road1Index].connectionPointInRoadIndex == 0 ? 0 : road1CurvePoints.size() - 1;
		int road2CenterPointIndex = _roads[road2Index].connectionPointInRoadIndex == 0 ? 0 : road2CurvePoints.size() - 1;
		glm::vec3 road1CenterPoint = road1CurvePoints[road1CenterPointIndex];
		glm::vec3 road2CenterPoint = road2CurvePoints[road2CenterPointIndex];

		glm::vec2 direction1 = RoadGenerator::calculateDirection(road1CurvePoints, temp, road1CenterPointIndex);
		glm::vec2 rightVector1 = glm::vec2(-direction1.y, direction1.x);

		glm::vec2 direction2 = RoadGenerator::calculateDirection(road2CurvePoints, temp, road2CenterPointIndex);
		glm::vec2 rightVector2 = glm::vec2(-direction2.y, direction2.x);

		if (_roads[road1Index].connectionPointInRoadIndex == 0)
		{
			rightVector1 = -rightVector1;
		}
		if (_roads[road2Index].connectionPointInRoadIndex == 1)
		{
			rightVector2 = -rightVector2;
		}
		
		shapePolygonComponent->addPoint(centerPoint);
		shapePolygonComponent->addPoint(road1CenterPoint);
		shapePolygonComponent->addPoint(road1CenterPoint + glm::vec3(rightVector1.x, 0.0f, rightVector1.y) * _width);
		shapePolygonComponent->addPoint(road2CenterPoint + glm::vec3(rightVector2.x, 0.0f, rightVector2.y) * _width);
		shapePolygonComponent->addPoint(road2CenterPoint);
	}

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
