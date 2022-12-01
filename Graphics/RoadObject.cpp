#include "RoadObject.h"

#include <algorithm>

#include "BezierCurve.h"
#include "CrossroadComponent.h"
#include "RoadIntersectionComponent.h"
#include "RoadGenerator.h"

#include "../Scene/SceneObject.h"

#include "../Utils/BezierCurvesUtils.h"
#include "../Utils/Logger.h"


RoadObject::RoadObject(RoadType roadType, RRoadProfile* _roadProfile, const std::vector<glm::vec3>& points, const std::vector<RoadSegment>& segments, bool buildModelAfterCreate)
	: _roadType(roadType), _roadProfile(_roadProfile), _points(points), _segments(segments), _connectionPoints(2), _connectionPointsData(2, nullptr),
	_buildModelAfterCreate(buildModelAfterCreate),
	_bezierCurveComponent(nullptr)
{
	_type = CT_ROAD_OBJECT;
}


RoadObject::RoadObject(RRoadProfile* _roadProfile, const std::vector<glm::vec3>& points, bool buildModelAfterCreate)
	: _roadType(RoadType::BEZIER_CURVES), _roadProfile(_roadProfile), _points(points), _connectionPoints(2), _connectionPointsData(2, nullptr),
	_buildModelAfterCreate(buildModelAfterCreate),
	_bezierCurveComponent(nullptr)
{
	_type = CT_ROAD_OBJECT;
}


RoadObject::~RoadObject()
{
	LOG_INFO("RoadObject destructor");
	if (_modelsDatas.size() > 0)
	{
		delete _modelsDatas[0].model;
	}

	setConnectionPoint(0, nullptr);
	setConnectionPoint(1, nullptr);
}


void RoadObject::onAttachedToScenObject()
{
	if (_roadType == RoadType::BEZIER_CURVES)
	{
		_bezierCurveComponent = dynamic_cast<BezierCurve*>(getSceneObject()->getComponent(CT_BEZIER_CURVE));
		if (_bezierCurveComponent == nullptr)
		{
			LOG_ERROR("Cannot find BezierCurve component in object!");
			return;
		}

		_bezierCurveComponent->setOnPointAddedListener(std::bind(&RoadObject::onPointAdded, this));
		_bezierCurveComponent->setOnPointDeletedListener(std::bind(&RoadObject::onPointDeleted, this, std::placeholders::_1));
		_bezierCurveComponent->setOnPointChangedPositionListener(std::bind(&RoadObject::onPointChangedPosition, this, std::placeholders::_1, std::placeholders::_2));
		_bezierCurveComponent->setOnCurveChangedListener([this]() { buildModel(); });
		_bezierCurveComponent->setOnComponentDeletedListener([this]()
			{
				_bezierCurveComponent = nullptr;
			});
	}

	if (_buildModelAfterCreate)
		buildModel();
}


bool RoadObject::isConnectionExist(int index)
{
	return _connectionPoints[index].crossroadComponent != nullptr || _connectionPoints[index].roadIntersectionComponent != nullptr;
}


void RoadObject::buildModel(bool reuseExistingModel)
{
	if (_modelsDatas[0].modelRootNode != nullptr)
	{
		delete _modelsDatas[0].modelRootNode;
		_modelsDatas[0].modelRootNode = nullptr;
	}

	if (!reuseExistingModel && _modelsDatas[0].model != nullptr)
	{
		delete _modelsDatas[0].model;
		_modelsDatas[0].model = nullptr;
	}

	if (_roadType == RoadType::LINES_AND_ARC)
	{
		buildModelLinesAndArcMode(_connectionPointsData);
	}
	else if (_roadType == RoadType::BEZIER_CURVES)
	{
		buildModelBezierCurvesMode(_connectionPointsData);
	}
	else if (_roadType == RoadType::POINTS)
	{
		auto tempPoints = _points;
		buildModelPoints(tempPoints, _connectionPointsData);
	}
}


void RoadObject::buildModelLinesAndArcMode(std::vector<RoadConnectionPointData*>& connectionPointsData)
{
	setModel(createRoadModel(_roadProfile->getRoadLanes(), _points, _segments, connectionPointsData, _modelsDatas[0].model));
}


void RoadObject::buildModelBezierCurvesMode(std::vector<RoadConnectionPointData*>& connectionPointsData)
{
	buildModelPoints(_bezierCurveComponent->getCurvePoints(), connectionPointsData);
}


void RoadObject::buildModelPoints(const std::vector<glm::vec3>& curvePoints, std::vector<RoadConnectionPointData*>& connectionPointsData)
{
	setModel(RoadGenerator::createRoadModel(_roadProfile->getRoadLanes(), curvePoints, connectionPointsData, _modelsDatas[0].model));
}


RoadType RoadObject::getRoadType()
{
	return _roadType;
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
	if (_roadType == RoadType::LINES_AND_ARC)
	{
		_points.push_back(position);

		if (_points.size() > 1)
		{
			_segments.push_back(RoadSegment());
		}
	}
	else if (_roadType == RoadType::BEZIER_CURVES)
	{
		LOG_WARNING("Impossible call to addPoint for BEZIER_CURVE road type.");
	}
}


void RoadObject::deletePoint(unsigned int index)
{
	if (index >= _points.size())
	{
		return;
	}

	if (_roadType == RoadType::LINES_AND_ARC)
	{
		_points.erase(_points.begin() + index);

		unsigned int segmentIndex = std::max((int)index - 1, 0);

		if (segmentIndex < _segments.size())
		{
			_segments.erase(_segments.begin() + segmentIndex);
		}
	}
	else if (_roadType == RoadType::BEZIER_CURVES)
	{
		LOG_WARNING("Impossible call to deletePoint for BEZIER_CURVE road type.");
	}
}


void RoadObject::setPointPostion(int index, glm::vec3 newPosition)
{
	if (index >= _points.size())
	{
		return;
	}

	if (_roadType == RoadType::LINES_AND_ARC)
	{
		_points[index] = newPosition;
	}

	if (_roadType == RoadType::BEZIER_CURVES)
	{
		LOG_WARNING("Impossible call to setPointPostion for BEZIER_CURVE road type.");
	}
}


void RoadObject::setConnectionPoint(int index, CrossroadComponent* crossroadComponent, int indexInCrossroad)
{
	if (index >= 2)
		return;

	if (_bezierCurveComponent != nullptr)
	{
		int connectedPointIndex = index == 0 ? 0 : _bezierCurveComponent->getPoints().size() - 1;
		if (crossroadComponent != nullptr)
		{
			_bezierCurveComponent->lockPointPosition(connectedPointIndex);
		}
		else
		{
			_bezierCurveComponent->unlockPointPosition(connectedPointIndex);
		}
	}

	if (_connectionPoints[index].crossroadComponent != nullptr)
	{
		_connectionPoints[index].crossroadComponent->disconnectRoad(_connectionPoints[index].index, this);
	}
	if (_connectionPoints[index].roadIntersectionComponent != nullptr)
	{
		_connectionPoints[index].roadIntersectionComponent->disconnectRoad(this, index);
	}

	if (crossroadComponent != nullptr)
	{
		crossroadComponent->connectRoad(indexInCrossroad, this);
	}

	_connectionPoints[index].roadIntersectionComponent = nullptr;
	_connectionPoints[index].crossroadComponent = crossroadComponent;
	_connectionPoints[index].index = indexInCrossroad;

	if (_connectionPoints[index].crossroadComponent != nullptr)
	{
		setConnectedPointPosition(index);
	}

	// build connection point data
	if (_connectionPoints[index].crossroadComponent != nullptr)
	{
		if (_connectionPointsData[index] == nullptr)
		{
			_connectionPointsData[index] = new RoadConnectionPointData;
		}

		glm::vec3 pos = _connectionPoints[index].crossroadComponent->getConnectionPoint(_connectionPoints[index].index).position;
		_connectionPointsData[index]->position = _connectionPoints[index].crossroadComponent->getSceneObject()->transformLocalPointToGlobal(pos);

		glm::vec3 dir = _connectionPoints[index].crossroadComponent->getConnectionPoint(_connectionPoints[index].index).direction;
		_connectionPointsData[index]->direction = _connectionPoints[index].crossroadComponent->getSceneObject()->transformLocalVectorToGlobal(dir);
	}
	else
	{
		if (_connectionPointsData[index] != nullptr)
		{
			delete _connectionPointsData[index];
		}
		_connectionPointsData[index] = nullptr;
	}
}


void RoadObject::setConnectionPointWithRoadIntersection(int index, RoadIntersectionComponent* roadIntersectionComponent)
{
	if (index >= 2)
		return;

	if (_bezierCurveComponent != nullptr)
	{
		int connectedPointIndex = index == 0 ? 0 : _bezierCurveComponent->getPoints().size() - 1;
		if (roadIntersectionComponent != nullptr)
		{
			_bezierCurveComponent->lockPointPosition(connectedPointIndex);
		}
		else
		{
			_bezierCurveComponent->unlockPointPosition(connectedPointIndex);
		}
	}

	if (_connectionPoints[index].crossroadComponent != nullptr)
	{
		_connectionPoints[index].crossroadComponent->disconnectRoad(_connectionPoints[index].index, this);
	}
	if (_connectionPoints[index].roadIntersectionComponent != nullptr)
	{
		_connectionPoints[index].roadIntersectionComponent->disconnectRoad(this, index);
	}

	if (roadIntersectionComponent != nullptr)
	{
		roadIntersectionComponent->connectRoad(this, index);
	}

	_connectionPoints[index].roadIntersectionComponent = roadIntersectionComponent;
	_connectionPoints[index].crossroadComponent = nullptr;
	_connectionPoints[index].index = 0;

	if (_connectionPoints[index].roadIntersectionComponent != nullptr)
	{
		setConnectedPointPosition(index);
	}
}


RoadConnectionPoint& RoadObject::getConnectionPoint(int index)
{
	return _connectionPoints[index];
}


void RoadObject::setCustomConnectionPointData(int index, const glm::vec3 pos, const glm::vec3 dir)
{
	if (index >= 2)
		return;

	if (_connectionPointsData[index] == nullptr)
	{
		_connectionPointsData[index] = new RoadConnectionPointData;
	}

	_connectionPointsData[index]->position = pos;
	_connectionPointsData[index]->direction = dir;
}


void RoadObject::setConnectedPointPosition(int connectionPointIndex)
{
	CrossroadComponent* crossroadComponent = _connectionPoints[connectionPointIndex].crossroadComponent;
	int indexInCrossroad = _connectionPoints[connectionPointIndex].index;

	RoadIntersectionComponent* roadIntersectionComponent = _connectionPoints[connectionPointIndex].roadIntersectionComponent;

	if (_roadType == RoadType::LINES_AND_ARC)
	{
		int pointIndex;
		if (connectionPointIndex == 0)
		{
			pointIndex = 0;
		}
		else if (connectionPointIndex == 1)
		{
			pointIndex = _points.size() - 1;
		}

		_points[pointIndex] = crossroadComponent != nullptr ?
			crossroadComponent->getGlobalPositionOfConnectionPoint(indexInCrossroad) :
			roadIntersectionComponent->getSceneObject()->getPosition();
	}
	else if (_roadType == RoadType::BEZIER_CURVES)
	{
		const auto& points = _bezierCurveComponent->getPoints();

		int pointIndex;
		int controlPointIndex;
		if (connectionPointIndex == 0)
		{
			pointIndex = 0;
			controlPointIndex = 1;
		}
		else if (connectionPointIndex == 1)
		{
			pointIndex = points.size() - 1;
			controlPointIndex = pointIndex - 1;
		}

		_bezierCurveComponent->unlockPointPosition(pointIndex);
		if (crossroadComponent != nullptr)
		{
			glm::vec3 newPosition = crossroadComponent->getGlobalPositionOfConnectionPoint(indexInCrossroad);
			glm::vec3 direction = crossroadComponent->getGlobalDirectionOfConnectionPoint(indexInCrossroad);

			float length = glm::length(points[pointIndex] - points[controlPointIndex]);

			_bezierCurveComponent->setPointPostion(pointIndex, newPosition);
			_bezierCurveComponent->setPointPostion(controlPointIndex, newPosition + length * direction);
		}
		else
		{
			_bezierCurveComponent->setPointPostion(pointIndex, roadIntersectionComponent->getSceneObject()->getPosition());
		}
		_bezierCurveComponent->lockPointPosition(pointIndex);
	}
}


void RoadObject::resetConnectedPointPositions()
{
	for (int i = 0; i < 2; ++i)
	{
		if (isConnectionExist(i))
		{
			setConnectedPointPosition(i);
		}
	}
}


const std::vector<glm::vec3>& RoadObject::getCurvePoints()
{
	return _bezierCurveComponent->getCurvePoints();
}


void RoadObject::onPointAdded()
{
	buildModel();
}


void RoadObject::onPointDeleted(int index)
{
	if (index == 0)
	{
		setConnectionPoint(0, nullptr);
	}
	else if (index > _bezierCurveComponent->getPoints().size())
	{
		setConnectionPoint(1, nullptr);
	}

	buildModel();
}


void RoadObject::onPointChangedPosition(int index, const glm::vec3& newPosition)
{
	//rebuild road intersections
	if (_connectionPoints[0].roadIntersectionComponent != nullptr && index < 5)
	{
		_connectionPoints[0].roadIntersectionComponent->needRebuild();
	}
	if (_connectionPoints[1].roadIntersectionComponent != nullptr && index > static_cast<int>(_bezierCurveComponent->getPoints().size()) - 6)
	{
		_connectionPoints[1].roadIntersectionComponent->needRebuild();
	}

	buildModel();
}
